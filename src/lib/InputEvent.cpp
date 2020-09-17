#include "./InputEvent.h"

InputEvent::InputEvent(KeyCode keyCode, KeyState keyState)
    : keyCode(keyCode)
    , keyState(keyState)
{
}

bool InputEvent::isDown() const
{
    return keyState == KeyState::DOWN
        || keyState == KeyState::REPEAT;
}

bool InputEvent::isUp() const
{
    return keyState == KeyState::UP;
}
