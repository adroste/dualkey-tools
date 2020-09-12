#pragma once

#include "../InputEvent.h"
#include "../ModifierFlags.h"
#include <vector>

class IKeyEventHandler {
public:
    virtual bool handleEvent(bool handled, const InputEvent& input, ModifierFlags modifierFlags, std::vector<InputEvent>& outputs) = 0;
};
