#pragma once

#include "../handler/IKeyEventHandler.h"

class IKeyHook {
public:
    virtual bool hookSync(IKeyEventHandler& handler) = 0;
    virtual void abort() = 0;
};
