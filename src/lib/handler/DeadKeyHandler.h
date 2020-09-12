#pragma once

#include "../UnicodeSymbol.h"
#include "./IKeyEventHandler.h"

class DeadKeyHandler : public IKeyEventHandler {
public:
    DeadKeyHandler(KeyCode deadKey, const std::vector<UnicodeSymbol>& symbols)
        : _deadKey(deadKey)
        , _symbols(symbols)
    {
    }

    bool handleEvent(bool handled, const InputEvent& input, ModifierFlags modifierFlags, std::vector<InputEvent>& outputs)
    {
        if (handled) {
            _doInsert = false;
            return false;
        }

        // ignore shift buttons
        if (input.keyCode == KeyCode::SHIFT_LEFT
            || input.keyCode == KeyCode::SHIFT_RIGHT)
            return false;
        
        if (input.keyCode == _deadKey) {
            _doInsert = true;
            return true;
        }

        if (_doInsert && input.isDown()) {
            _doInsert = false;
            for (const auto& symbol : _symbols) {
                if (symbol.matches(input.keyCode, modifierFlags)) {
                    symbol.insertSymbol(outputs);
                    return true;
                }
            }
        }

        return false;
    }

private:
    bool _doInsert = false;
    KeyCode _deadKey;
    const std::vector<UnicodeSymbol> _symbols;
};
