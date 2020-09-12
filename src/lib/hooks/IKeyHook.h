#pragma once

#include "../handler/IKeyEventHandler.h"

class IKeyHook {
public:
    virtual bool hookSync(IKeyEventHandler* pHandler) = 0;
    virtual void clearHook() = 0;
};
