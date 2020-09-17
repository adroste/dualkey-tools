#pragma once

#include "./InputEvent.h"
#include "./KeyCode.h"
#include "./KeyState.h"
#include "./ModifierFlags.h"
#include <vector>

/*
 * Dev Notes:
 * to use linux compose key
 * keyboard file: `/etc/default/keyboard`
 * set temp: `setxkbmap -option compose:102`
 * check config: `setxkbmap -query`
 */

class CustomSymbol {
public:
    KeyCode triggerKey;
    std::vector<ModifierFlags> matchingModifierFlags;
    std::vector<std::vector<KeyCode>> keySequence;

    CustomSymbol(KeyCode triggerKey, const std::vector<ModifierFlags>& matchingModifierFlags, const std::vector<std::vector<KeyCode>>& keySequence);

    void insertSymbol(std::vector<InputEvent>& outputs) const;

    bool matches(KeyCode keyCode, ModifierFlags modifierFlags) const;
};
