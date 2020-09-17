#pragma once

#include "./IKeyEventHandler.h"
#include <memory>

class HandlerComposer : public IKeyEventHandler {
public:
    std::vector<std::unique_ptr<IKeyEventHandler>> keyHandler;

    bool handleEvent(bool handled, const InputEvent& input, ModifierFlags modifierFlags, std::vector<InputEvent>& outputs);
};
