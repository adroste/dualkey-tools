#pragma once

enum class ModifierFlags {
    NONE = 0,
    SHIFT = 1,
    CAPS_LOCK = 2,
};

inline ModifierFlags operator|=(ModifierFlags& a, ModifierFlags b)
{
    a = static_cast<ModifierFlags>(static_cast<int>(a) | static_cast<int>(b));
    return a;
}

inline ModifierFlags operator|(ModifierFlags a, ModifierFlags b)
{
    return a |= b;
}
