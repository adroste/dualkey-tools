#pragma once

#include "./KeyCode.h"
#include "./KeyState.h"

class InputEvent {
public:
    KeyCode keyCode;
    KeyState keyState;

    InputEvent(KeyCode keyCode, KeyState keyState);

    bool isDown() const;

    bool isUp() const;
};
