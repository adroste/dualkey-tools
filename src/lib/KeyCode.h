#pragma once

#include <linux/input-event-codes.h>

enum class KeyCode {
    CAPS_LOCK = KEY_CAPSLOCK,
    CTRL_LEFT = KEY_LEFTCTRL,
    CTRL_RIGHT = KEY_RIGHTCTRL,
    SHIFT_LEFT = KEY_LEFTSHIFT,
    SHIFT_RIGHT = KEY_RIGHTSHIFT,
    ALT_LEFT = KEY_LEFTALT,
    ALT_RIGHT = KEY_RIGHTALT,
    WIN_LEFT = KEY_LEFTMETA,
    WIN_RIGHT = KEY_RIGHTMETA,
};

inline bool isModifier(KeyCode k) {
    return k == KeyCode::CTRL_LEFT
        || k == KeyCode::CTRL_RIGHT
        || k == KeyCode::SHIFT_LEFT
        || k == KeyCode::SHIFT_RIGHT
        || k == KeyCode::ALT_LEFT
        || k == KeyCode::ALT_RIGHT
        || k == KeyCode::WIN_LEFT
        || k == KeyCode::WIN_RIGHT;
}
