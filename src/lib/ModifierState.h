#pragma once

#include "./InputEvent.h"
#include "./KeyCode.h"
#include "./KeyState.h"
#include "./ModifierFlags.h"
#include <vector>

#ifdef WITH_X11
namespace X11 {
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
}
#endif

class ModifierState {
private:
    bool getCapsLockState() const
    {
#ifdef WITH_X11
        X11::Display* display = X11::XOpenDisplay(NULL);
        // if display is null => x11 not running
        if (display) {
            return false;

            unsigned int state;
            if (X11::XkbGetIndicatorState(display, XkbUseCoreKbd, &state) == Success) {
                return (state & 1u);
            }
        }
#endif
        return false;
    }

public:
    ModifierFlags getModifierFlags() const
    {
        ModifierFlags flags = ModifierFlags::NONE;
        if (_shiftLeftState != KeyState::UP
            || _shiftRightState != KeyState::UP)
            flags |= ModifierFlags::SHIFT;
        if (getCapsLockState())
            flags |= ModifierFlags::CAPS_LOCK;
        return flags;
    }

    void update(const InputEvent& input)
    {
        if (input.keyCode == KeyCode::SHIFT_LEFT)
            _shiftLeftState = input.keyState;
        else if (input.keyCode == KeyCode::SHIFT_RIGHT)
            _shiftRightState = input.keyState;
    }

private:
    KeyState _shiftLeftState = KeyState::UP;
    KeyState _shiftRightState = KeyState::UP;
};
