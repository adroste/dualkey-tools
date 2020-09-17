#pragma once

enum class ModifierFlags {
    NONE = 0,
    SHIFT = 1,
    // currently not supported
    /* CAPS_LOCK = 2, */ 
    CTRL = 4, 
    ALT = 8, 
    ALT_GR = 16, 
    WIN = 32,
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
