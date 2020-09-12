#pragma once

#include "./IKeyEventHandler.h"
#include <stdio.h>
#include <cstdlib>

class DebugKeyHandler : public IKeyEventHandler {
public:
    bool handleEvent(bool, const InputEvent& input, ModifierFlags modifierFlags, std::vector<InputEvent>& outputs)
    {
        printf("%d, %d, %d\n", static_cast<int>(input.keyCode), static_cast<int>(input.keyState), static_cast<int>(modifierFlags));
        if (input.keyCode == KeyCode::Q)
            std::exit(0);

        outputs.push_back(input);
        return true;
    }
};
