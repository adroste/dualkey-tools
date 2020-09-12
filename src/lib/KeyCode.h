#pragma once

#include <linux/input.h>

enum class KeyCode {
    COMPOSE = KEY_COMPOSE,
    APOSTROPHE = KEY_APOSTROPHE,
    ISO_EXTRA = KEY_102ND,
    CAPS_LOCK = KEY_CAPSLOCK,
    CTRL_LEFT = KEY_LEFTCTRL,
    CTRL_RIGHT = KEY_RIGHTCTRL,
    ENTER = KEY_ENTER,
    ESC = KEY_ESC,
    SHIFT_LEFT = KEY_LEFTSHIFT,
    SHIFT_RIGHT = KEY_RIGHTSHIFT,
    SPACE = KEY_SPACE,

    N0 = KEY_0,
    N2 = KEY_2,
    N4 = KEY_4,
    N6 = KEY_6,
    N9 = KEY_9,
    A = KEY_A,
    C = KEY_C,
    D = KEY_D,
    E = KEY_E,
    F = KEY_F,
    O = KEY_O,
    Q = KEY_Q,
    S = KEY_S,
    U = KEY_U,
};
