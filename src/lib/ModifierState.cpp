#include "./ModifierState.h"

ModifierFlags ModifierState::getModifierFlags() const
{
    ModifierFlags flags = ModifierFlags::NONE;
    if (_shiftLeftState != KeyState::UP
        || _shiftRightState != KeyState::UP)
        flags |= ModifierFlags::SHIFT;
    if (_ctrlLeftState != KeyState::UP
        || _ctrlRightState != KeyState::UP)
        flags |= ModifierFlags::CTRL;
    if (_winLeftState != KeyState::UP
        || _winRightState != KeyState::UP)
        flags |= ModifierFlags::WIN;
    if (_altLeftState != KeyState::UP)
        flags |= ModifierFlags::ALT;
    if (_altRightState != KeyState::UP)
        flags |= ModifierFlags::ALT_GR;
    return flags;
}

void ModifierState::update(const InputEvent& input)
{
    if (input.keyCode == KeyCode::SHIFT_LEFT)
        _shiftLeftState = input.keyState;
    else if (input.keyCode == KeyCode::SHIFT_RIGHT)
        _shiftRightState = input.keyState;
    else if (input.keyCode == KeyCode::CTRL_LEFT)
        _ctrlLeftState = input.keyState;
    else if (input.keyCode == KeyCode::CTRL_RIGHT)
        _ctrlRightState = input.keyState;
    else if (input.keyCode == KeyCode::WIN_LEFT)
        _winLeftState = input.keyState;
    else if (input.keyCode == KeyCode::WIN_RIGHT)
        _winRightState = input.keyState;
    else if (input.keyCode == KeyCode::ALT_LEFT)
        _altLeftState = input.keyState;
    else if (input.keyCode == KeyCode::ALT_RIGHT)
        _altRightState = input.keyState;
}
