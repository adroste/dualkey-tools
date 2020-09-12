#pragma once

#include "./KeyCode.h"
#include "./KeyState.h"

class InputEvent {
public:
    KeyCode keyCode;
    KeyState keyState;

    InputEvent(KeyCode keyCode, KeyState keyState)
        : keyCode(keyCode)
        , keyState(keyState)
    {
    }

    bool isDown() const
    {
        return keyState == KeyState::DOWN
            || keyState == KeyState::REPEAT;
    }

    bool isUp() const
    {
        return keyState == KeyState::UP;
    }
};
