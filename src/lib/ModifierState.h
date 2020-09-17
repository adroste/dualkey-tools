#pragma once

#include "./InputEvent.h"
#include "./KeyCode.h"
#include "./KeyState.h"
#include "./ModifierFlags.h"
#include <vector>

// caps lock detection is not supported at the moment
class ModifierState {
public:
    ModifierFlags getModifierFlags() const;

    void update(const InputEvent& input);

private:
    KeyState _shiftLeftState = KeyState::UP;
    KeyState _shiftRightState = KeyState::UP;
    KeyState _ctrlLeftState = KeyState::UP;
    KeyState _ctrlRightState = KeyState::UP;
    KeyState _winLeftState = KeyState::UP;
    KeyState _winRightState = KeyState::UP;
    KeyState _altLeftState = KeyState::UP;
    KeyState _altRightState = KeyState::UP;
};
