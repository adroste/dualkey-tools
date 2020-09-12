#include <memory>
#include <stdio.h>
#include <vector>

#include "./lib/hooks/LinuxKeyHook.h"
#include "./lib/ModifierState.h"
#include "./lib/handler/DeadKeyHandler.h"
#include "./lib/handler/DualKeyHandler.h"
#include "./lib/handler/DebugKeyHandler.h"

/*
 * Pre-defined key handler instances
 */
DebugKeyHandler debugKeyHandler; 

DeadKeyHandler deadKeyGermanHandler(
    KeyCode::ISO_EXTRA,
    GERMAN_SYMBOLS);

DualKeyHandler capsLockToEscAndCtrl(
    KeyCode::CAPS_LOCK,
    { KeyCode::ESC },
    { KeyCode::CTRL_LEFT },
    {});

DualKeyHandler ctrlToCapsLockAndCtrl(
    KeyCode::CTRL_LEFT,
    { KeyCode::CAPS_LOCK },
    { KeyCode::CTRL_LEFT },
    {});

DualKeyHandler enterToEnterAndCtrl(
    KeyCode::ENTER,
    { KeyCode::ENTER },
    { KeyCode::CTRL_RIGHT },
    {});

DualKeyHandler shiftLeftToLeftParenthesesAndShiftLeft(
    KeyCode::SHIFT_LEFT,
    { KeyCode::SHIFT_LEFT, KeyCode::N9 },
    { KeyCode::SHIFT_LEFT },
    { KeyCode::SHIFT_RIGHT });

DualKeyHandler shiftRightToRightParenthesesAndShiftRight(
    KeyCode::SHIFT_RIGHT,
    { KeyCode::SHIFT_RIGHT, KeyCode::N0 },
    { KeyCode::SHIFT_RIGHT },
    { KeyCode::SHIFT_LEFT });

int main(void)
{
    // TODO mutex

    setbuf(stdin, NULL), setbuf(stdout, NULL);

    LinuxKeyHook keyHook;

    std::vector<IKeyEventHandler*> handler;
    handler.push_back(&capsLockToEscAndCtrl);
    handler.push_back(&enterToEnterAndCtrl);
    handler.push_back(&ctrlToCapsLockAndCtrl);
    handler.push_back(&deadKeyGermanHandler);
    handler.push_back(&shiftLeftToLeftParenthesesAndShiftLeft);
    handler.push_back(&shiftRightToRightParenthesesAndShiftRight);

    keyHook.hookSync(&debugKeyHandler);

    /* bool handled; */
    /* ModifierFlags modifierFlags; */
    /* while (interceptor.readEvent(p_input)) { */
    /*     handled = false; */
    /*     modifierState.update(*p_input); */
    /*     modifierFlags = modifierState.getModifierFlags(); */

    /*     // order = priority */
    /*     for (auto& h : handler) */
    /*         handled |= h->handleEvent(handled, *p_input, modifierFlags, outputs); */
    /*     // forward unhandled input */
    /*     // DO NOT REMOVE or your keyboard will not work anymore */
    /*     if (!handled) */
    /*         outputs.push_back(*p_input); */

    /*     interceptor.writeEvents(outputs); */
    /*     outputs.clear(); */
    /* } */
}
