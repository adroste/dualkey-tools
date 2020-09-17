#include "./CustomSymbol.h"

CustomSymbol::CustomSymbol(KeyCode triggerKey, const std::vector<ModifierFlags>& matchingModifierFlags, const std::vector<std::vector<KeyCode>>& keySequence)
    : triggerKey(triggerKey)
    , matchingModifierFlags(matchingModifierFlags)
    , keySequence(keySequence)
{
}

void CustomSymbol::insertSymbol(std::vector<InputEvent>& outputs) const
{
    for (const auto& keys : keySequence) {
        for (const auto& key : keys)
            outputs.push_back(InputEvent(key, KeyState::DOWN));
        for (const auto& key : keys)
            outputs.push_back(InputEvent(key, KeyState::UP));
    }
}

bool CustomSymbol::matches(KeyCode keyCode, ModifierFlags modifierFlags) const
{
    if (keyCode == triggerKey) {
        for (auto flags : matchingModifierFlags) {
            if (flags == modifierFlags)
                return true;
        }
    }
    return false;
}
