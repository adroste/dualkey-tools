#pragma once

#include "./IKeyEventHandler.h"

class DualKeyHandler : public IKeyEventHandler {
public:
    KeyCode triggerKey;
    std::vector<std::vector<KeyCode>> tapKeySequence;
    std::vector<KeyCode> holdKeys;
    std::vector<KeyCode> ignoredKeys;

    DualKeyHandler(KeyCode triggerKey, const std::vector<std::vector<KeyCode>>& tapKeySequence, const std::vector<KeyCode>& holdKeys, const std::vector<KeyCode>& ignoredKeys);

    bool handleEvent(bool handled, const InputEvent& input, ModifierFlags, std::vector<InputEvent>& outputs);

private:
    bool isIgnored(KeyCode key);

    bool _triggerDown = false;
    bool _pressedAlone = true;
};
