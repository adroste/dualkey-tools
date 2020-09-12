#pragma once

#include "./IKeyEventHandler.h"

class DualKeyHandler : public IKeyEventHandler {
public:
    KeyCode triggerKey;
    std::vector<KeyCode> tapKeys;
    std::vector<KeyCode> holdKeys;
    std::vector<KeyCode> ignoredKeys;

    DualKeyHandler(KeyCode triggerKey, const std::vector<KeyCode>& tapKeys, const std::vector<KeyCode>& holdKeys, const std::vector<KeyCode>& ignoredKeys)
        : triggerKey(triggerKey)
        , tapKeys(tapKeys)
        , holdKeys(holdKeys)
        , ignoredKeys(ignoredKeys)
    {
    }

    bool isIgnored(KeyCode key)
    {
        for (const auto& k : ignoredKeys) {
            if (key == k)
                return true;
        }
        return false;
    }

    bool handleEvent(bool handled, const InputEvent& input, ModifierFlags, std::vector<InputEvent>& outputs)
    {
        if (_triggerDown) {
            if (input.keyCode == triggerKey && input.isUp()) {
                if (_pressedAlone) {
                    for (const auto& tapKey : tapKeys)
                        outputs.push_back(InputEvent(tapKey, KeyState::DOWN));
                    for (const auto& tapKey : tapKeys)
                        outputs.push_back(InputEvent(tapKey, KeyState::UP));
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

private:
    bool _triggerDown = false;
    bool _pressedAlone = true;
};
