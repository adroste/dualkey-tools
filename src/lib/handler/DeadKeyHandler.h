#pragma once

#include "../CustomSymbol.h"
#include "./IKeyEventHandler.h"

class DeadKeyHandler : public IKeyEventHandler {
public:
    DeadKeyHandler(KeyCode deadKey, const std::vector<CustomSymbol>& symbols);

    bool handleEvent(bool handled, const InputEvent& input, ModifierFlags modifierFlags, std::vector<InputEvent>& outputs);

private:
    bool _doInsert = false;
    KeyCode _deadKey;
    const std::vector<CustomSymbol> _symbols;
};
