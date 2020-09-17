#include "./DualKeyHandler.h"

DualKeyHandler::DualKeyHandler(KeyCode triggerKey, const std::vector<std::vector<KeyCode>>& tapKeySequence, const std::vector<KeyCode>& holdKeys, const std::vector<KeyCode>& ignoredKeys)
    : triggerKey(triggerKey)
    , tapKeySequence(tapKeySequence)
    , holdKeys(holdKeys)
    , ignoredKeys(ignoredKeys)
{
}

bool DualKeyHandler::isIgnored(KeyCode key)
{
    for (const auto& k : ignoredKeys) {
        if (key == k)
            return true;
    }
    return false;
}

bool DualKeyHandler::handleEvent(bool handled, const InputEvent& input, ModifierFlags, std::vector<InputEvent>& outputs)
{
    if (_triggerDown) {
        if (input.keyCode == triggerKey && input.isUp()) {
            if (_pressedAlone) {
                for (const auto& keys : tapKeySequence) {
                    for (const auto& key : keys)
                        outputs.push_back(InputEvent(key, KeyState::DOWN));
                    for (const auto& key : keys)
                        outputs.push_back(InputEvent(key, KeyState::UP));
                }
            } else {
                _pressedAlone = true;
                for (const auto& holdKey : holdKeys)
                    outputs.push_back(InputEvent(holdKey, KeyState::UP));
            }
            _triggerDown = false;
            return true;
        }

        if (_pressedAlone && input.isDown() && !isIgnored(input.keyCode)) {
            _pressedAlone = false;
            for (const auto& holdKey : holdKeys)
                outputs.insert(outputs.begin(), InputEvent(holdKey, KeyState::DOWN));
            if (!handled)
                outputs.push_back(input);
            return true;
        }
    } else if (!handled
        && input.keyCode == triggerKey
        && input.isDown()) {
        _triggerDown = true;
        return true;
    }

    return false;
}
