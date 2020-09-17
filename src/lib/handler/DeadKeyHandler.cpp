#include "./DeadKeyHandler.h"

DeadKeyHandler::DeadKeyHandler(KeyCode deadKey, const std::vector<CustomSymbol>& symbols)
    : _deadKey(deadKey)
    , _symbols(symbols)
{
}

bool DeadKeyHandler::handleEvent(bool handled, const InputEvent& input, ModifierFlags modifierFlags, std::vector<InputEvent>& outputs)
{
    if (handled) {
        _doInsert = false;
        return false;
    }

    if (input.keyCode == _deadKey) {
        if (input.isDown())
            _doInsert = true;
        return true;
    }

    if (_doInsert && input.isDown()) {
        for (const auto& symbol : _symbols) {
            if (symbol.matches(input.keyCode, modifierFlags)) {
                symbol.insertSymbol(outputs);
                _doInsert = false;
                return true;
            }
        }
        // ignore modifiers / end insert mode on non-modifier key
        if (!isModifier(input.keyCode))
            _doInsert = false;
    }

    return false;
}
