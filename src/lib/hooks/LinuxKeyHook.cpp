#include "./LinuxKeyHook.h"
#include "../ModifierState.h"
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

bool LinuxKeyHook::hookSync(IKeyEventHandler& handler)
{
    if (!init()) {
        clearHook();
        return false;
    }

    std::vector<InputEvent> outputs;
    std::unique_ptr<InputEvent> pInp;
    ModifierState modifierState;
    ModifierFlags modifierFlags;

    // deviceFd can be used to implement stateful handler seperated by device
    // e.g. different ModifierState objects for each keyboard
    // currently deviceId is ignored and everything is global
    int deviceFd;
    while ((deviceFd = readEvent(pInp)) != -1) {
        modifierState.update(*pInp);
        modifierFlags = modifierState.getModifierFlags();

        if (!handler.handleEvent(false, *pInp, modifierFlags, outputs))
            // forward unhandled input
            // DO NOT REMOVE or your keyboard will not work anymore
            outputs.push_back(*pInp);

        if (!writeEvents(outputs))
            break; // err
        outputs.clear();
    }

    clearHook();
    return false;
}

void LinuxKeyHook::abort() {
    clearHook();
}

int LinuxKeyHook::readEvent(std::unique_ptr<InputEvent>& p_input)
{
    struct input_event nativeEvent;
    int deviceFd;
    while ((deviceFd = readNativeEvent(&nativeEvent)) != -1) {
        if (nativeEvent.type == EV_MSC && nativeEvent.code == MSC_SCAN)
            continue;

        if (nativeEvent.type != EV_KEY) {
            writeNativeEvent(nativeEvent);
            continue;
        }

        p_input.reset(new InputEvent(static_cast<KeyCode>(nativeEvent.code), static_cast<KeyState>(nativeEvent.value)));
        return deviceFd;
    }
    return -1;
}

bool LinuxKeyHook::writeEvents(const std::vector<InputEvent>& events)
{
    for (const auto& event : events) {
        struct input_event nativeEvent = {
            .type = EV_KEY,
            .code = static_cast<KeyCodeType>(event.keyCode),
            .value = static_cast<KeyValueType>(event.keyState)
        };
        if (!writeNativeEvent(nativeEvent))
            return false;
        if (!writeNativeEvent(SYN_EVENT))
            return false;
        /* usleep(20000); */
    }
    return true;
}

int LinuxKeyHook::readNativeEvent(struct input_event* event)
{
    for (;;) {
        for (auto it = _devices.begin(); it != _devices.end(); ++it) {
            if (!libevdev_has_event_pending(it->second))
                continue;

            int rc = libevdev_next_event(it->second, LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING, event);

            while (rc == LIBEVDEV_READ_STATUS_SYNC)
                rc = libevdev_next_event(it->second, LIBEVDEV_READ_FLAG_SYNC, event);

            if (rc == -EAGAIN) // no data available
                continue;

            if (rc != LIBEVDEV_READ_STATUS_SUCCESS) {
                printf("Closed input device: %d\n", it->first);
                libevdev_grab(it->second, LIBEVDEV_UNGRAB);
                libevdev_free(it->second);
                close(it->first);
                _devices.erase(it);
                break;
            }

            return it->first;
        }

        monitorReceiveDevice();

        int maxFd = _udevMonitorFd;
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(_udevMonitorFd, &fds);
        for (const auto& d : _devices) {
            FD_SET(d.first, &fds);
        }

        if (select(maxFd + 1, &fds, nullptr, nullptr, nullptr) == -1) {
            if (errno == EAGAIN)
                continue;
            return -1;
        }
    }
}

bool LinuxKeyHook::writeNativeEvent(const struct input_event& event)
{
    if (!p_uInput)
        return false;

    if (libevdev_uinput_write_event(p_uInput, event.type, event.code, event.value) == 0)
        return true;

    return false;
}

bool LinuxKeyHook::init()
{
    clearHook();

    p_udev = udev_new();
    if (!p_udev) {
        clearHook();
        return false;
    }

    scanDevices();

    if (!setupMonitor()) {
        clearHook();
        return false;
    }

    if (!openUInputDev()) {
        clearHook();
        return false;
    }

    return true;
}

void LinuxKeyHook::clearHook()
{
    if (p_udevMonitor != nullptr) {
        udev_monitor_unref(p_udevMonitor);
        p_udevMonitor = nullptr;
    }
    _udevMonitorFd = 0;

    if (p_uInput != nullptr) {
        libevdev_uinput_destroy(p_uInput);
        p_uInput = nullptr;
    }

    if (p_udev != nullptr) {
        udev_unref(p_udev);
        p_udev = nullptr;
    }

    for (const auto& dp : _devices) {
        printf("Closed input device: %d\n", dp.first);
        libevdev_grab(dp.second, LIBEVDEV_UNGRAB);
        libevdev_free(dp.second);
        close(dp.first);
    }
    _devices.clear();
}

void LinuxKeyHook::monitorReceiveDevice()
{
    if (udev_device* device = udev_monitor_receive_device(p_udevMonitor)) {
        const char* action = udev_device_get_action(device);
        // TODO check if this is right
        if (action != nullptr && strcmp(action, "add") == 0) {
            const char* devNode = getDevNode(device);
            openDevNode(devNode);
        }
        udev_device_unref(device);
    }
}

bool LinuxKeyHook::setupMonitor()
{
    if (p_udev == nullptr)
        return false;

    p_udevMonitor = udev_monitor_new_from_netlink(p_udev, "udev");
    if (!p_udevMonitor)
        return false;

    udev_monitor_filter_add_match_subsystem_devtype(p_udevMonitor, "input", nullptr);

    if (udev_monitor_enable_receiving(p_udevMonitor) < 0) {
        udev_monitor_unref(p_udevMonitor);
        p_udevMonitor = nullptr;
        return false;
    }

    _udevMonitorFd = udev_monitor_get_fd(p_udevMonitor);
    if (_udevMonitorFd < 0) {
        udev_monitor_unref(p_udevMonitor);
        p_udevMonitor = nullptr;
        _udevMonitorFd = 0;
        return false;
    }
    return true;
}

const char* LinuxKeyHook::getDevNode(udev_device* device)
{
    if (device == nullptr)
        return nullptr;

    const char virtual_devices_directory[] = "/sys/devices/virtual/input/";
    if (strncmp(udev_device_get_syspath(device), virtual_devices_directory,
            sizeof(virtual_devices_directory) - 1)
        == 0)
        return nullptr;

    const char input_prefix[] = "/dev/input/event";
    const char* devnode = udev_device_get_devnode(device);
    if (!devnode || strncmp(devnode, input_prefix, sizeof(input_prefix) - 1))
        return nullptr;

    return devnode;
}

bool LinuxKeyHook::openUInputDev()
{
    if (p_uInput)
        return true;

    if (_devices.size() < 1)
        return false;

    auto dev = _devices[0].second;
    int err = libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &p_uInput);
    if (err == -EACCES) {
        printf("Could not open uinput: Permission denied\n");
        return false;
    }
    if (err != 0) {
        printf("Could not open uinput\n");
        return false;
    }

    printf("Opened uinput\n");
    return true;
}

bool LinuxKeyHook::openDevNode(const char* devNode)
{
    if (devNode == nullptr)
        return false;

    int fd = open(devNode, O_RDONLY);
    if (fd < 0)
        return false;

    struct libevdev* dev;
    if (libevdev_new_from_fd(fd, &dev) < 0) {
        close(fd);
        return false;
    }

    sleep(1);

    // check if keyboard
    if (!libevdev_has_event_type(dev, EV_KEY)
        || !libevdev_has_event_code(dev, EV_KEY, KEY_A)) {
        libevdev_free(dev);
        close(fd);
        return false;
    }

    printf("Found keyboard: %d (%s) - %s\n", fd, devNode, libevdev_get_name(dev));

    if (libevdev_grab(dev, LIBEVDEV_GRAB) < 0) {
        printf("Could not grab input device: %s\n", devNode);
        libevdev_free(dev);
        close(fd);
        return false;
    }

    printf("Opened input device: %d (%s)\n", fd, devNode);

    _devices.push_back(std::make_pair(fd, dev));
    return true;
}

void LinuxKeyHook::scanDevices()
{
    udev_enumerate* enumerate = udev_enumerate_new(p_udev);
    udev_enumerate_add_match_subsystem(enumerate, "input");
    udev_enumerate_scan_devices(enumerate);
    udev_list_entry* devices = udev_enumerate_get_list_entry(enumerate);
    udev_list_entry* dev_list_entry;
    udev_list_entry_foreach(dev_list_entry, devices)
    {
        if (udev_device* device = udev_device_new_from_syspath(
                p_udev, udev_list_entry_get_name(dev_list_entry))) {
            const char* devNode = getDevNode(device);
            openDevNode(devNode);
            udev_device_unref(device);
        }
    }
    udev_enumerate_unref(enumerate);
}
