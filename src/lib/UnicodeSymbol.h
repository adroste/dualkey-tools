#pragma once

#include "./InputEvent.h"
#include "./KeyCode.h"
#include "./KeyState.h"
#include "./ModifierFlags.h"
#include <vector>

/*
 * Dev Notes:
 * reload keyboard config: `sudo udevadm trigger --subsystem-match=input --action=change`
 * keyboard file: `/etc/default/keyboard`
 * set temp: `setxkbmap -option compose:102`
 * check config: `setxkbmap -query`
 */

class UnicodeSymbol {
public:
    KeyCode triggerKey;
    std::vector<ModifierFlags> matchingModifierFlags;
    std::vector<KeyCode> insertCodes;

    void insertSymbol(std::vector<InputEvent>& outputs) const
    {
        outputs.push_back(InputEvent(KeyCode::CTRL_LEFT, KeyState::DOWN));
        outputs.push_back(InputEvent(KeyCode::SHIFT_LEFT, KeyState::DOWN));
        outputs.push_back(InputEvent(KeyCode::U, KeyState::DOWN));
        outputs.push_back(InputEvent(KeyCode::U, KeyState::UP));
        outputs.push_back(InputEvent(KeyCode::SHIFT_LEFT, KeyState::UP));
        outputs.push_back(InputEvent(KeyCode::CTRL_LEFT, KeyState::UP));
        for (const auto& keyCode : insertCodes) {
            outputs.push_back(InputEvent(keyCode, KeyState::DOWN));
            outputs.push_back(InputEvent(keyCode, KeyState::UP));
        }
        outputs.push_back(InputEvent(KeyCode::SPACE, KeyState::DOWN));
        outputs.push_back(InputEvent(KeyCode::SPACE, KeyState::UP));
    }

    bool matches(KeyCode keyCode, ModifierFlags modifierFlags) const
    {
        if (keyCode == triggerKey) {
            for (auto flags : matchingModifierFlags) {
                if (flags == modifierFlags)
                    return true;
            }
        }
        return false;
    }
};

const std::vector<UnicodeSymbol> GERMAN_SYMBOLS = {
    // a umlaut
    { KeyCode::A, { ModifierFlags::SHIFT }, { KeyCode::N0, KeyCode::N0, KeyCode::C, KeyCode::N4 } },
    { KeyCode::A, { ModifierFlags::NONE }, { KeyCode::N0, KeyCode::N0, KeyCode::E, KeyCode::N4 } },
    // o umlaut
    { KeyCode::O, { ModifierFlags::SHIFT }, { KeyCode::N0, KeyCode::N0, KeyCode::D, KeyCode::N6 } },
    { KeyCode::O, { ModifierFlags::NONE }, { KeyCode::N0, KeyCode::N0, KeyCode::F, KeyCode::N6 } },
    // u umlaut
    { KeyCode::U, { ModifierFlags::SHIFT }, { KeyCode::N0, KeyCode::N0, KeyCode::D, KeyCode::C } },
    { KeyCode::U, { ModifierFlags::NONE }, { KeyCode::N0, KeyCode::N0, KeyCode::F, KeyCode::C } },
    // sharp s
    { KeyCode::S, { ModifierFlags::NONE }, { KeyCode::N0, KeyCode::N0, KeyCode::D, KeyCode::F } },
    // euro sign
    { KeyCode::N4, { ModifierFlags::SHIFT }, { KeyCode::N2, KeyCode::N0, KeyCode::A, KeyCode::C } },
};
