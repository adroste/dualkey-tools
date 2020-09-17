#pragma once

#include "./IKeyEventHandler.h"
#include <iostream>
#include <iomanip>

class OutputKeyCodeHandler : public IKeyEventHandler {
public:
    bool handleEvent(bool, const InputEvent& input, ModifierFlags, std::vector<InputEvent>& outputs);
};
