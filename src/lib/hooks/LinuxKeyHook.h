#pragma once

#include "../InputEvent.h"
#include "./IKeyHook.h"
#include <libevdev/libevdev-uinput.h>
#include <libevdev/libevdev.h>
#include <libudev.h>
#include <linux/input.h>
#include <memory>
#include <vector>

#define KeyCodeType unsigned short
#define KeyValueType int
const struct input_event SYN_EVENT = { .type = EV_SYN, .code = SYN_REPORT, .value = 0 };

class LinuxKeyHook : public IKeyHook {
public:
    bool hookSync(IKeyEventHandler& handler);
    void abort();

private:
    bool init();
    void clearHook();
    int readEvent(std::unique_ptr<InputEvent>& p_input);
    bool writeEvents(const std::vector<InputEvent>& events);

    int readNativeEvent(struct input_event* event);
    bool writeNativeEvent(const struct input_event& event);
    bool setupMonitor();
    void monitorReceiveDevice();
    bool openUInputDev();
    bool openDevNode(const char* devnode);
    void scanDevices();
    const char* getDevNode(udev_device* device);

    std::vector<std::pair<int /* fd */, libevdev*>> _devices;
    libevdev_uinput* p_uInput = nullptr;
    int _udevMonitorFd = 0;
    udev_monitor* p_udevMonitor = nullptr;
    udev* p_udev = nullptr;
};
