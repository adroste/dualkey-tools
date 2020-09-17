cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(dualkey-tools)

set(SOURCES
    main.cpp
    lib/ConfigLoader.cpp
    lib/CustomSymbol.cpp
    lib/InputEvent.cpp
    lib/ModifierState.cpp
    lib/hooks/LinuxKeyHook.cpp
    lib/handler/DeadKeyHandler.cpp
    lib/handler/DualKeyHandler.cpp
    lib/handler/HandlerComposer.cpp
    lib/handler/OutputKeyCodeHandler.cpp)

add_executable(dualkey-tools
    ${SOURCES}
    /usr/include/libevdev-1.0/libevdev/libevdev.h
    /usr/include/libevdev-1.0/libevdev/libevdev-uinput.h)
target_include_directories(dualkey-tools PRIVATE "/usr/include/libevdev-1.0")
target_compile_options(dualkey-tools PRIVATE -Wall -Wextra -Wno-missing-field-initializers)
target_link_libraries(dualkey-tools evdev udev yaml-cpp)
