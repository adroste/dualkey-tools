#include "./HandlerComposer.h"

bool HandlerComposer::handleEvent(bool handled, const InputEvent& input, ModifierFlags modifierFlags, std::vector<InputEvent>& outputs)
{
    bool handledHere = false;
    for (auto& h : keyHandler)
        handledHere |= h->handleEvent(handled || handledHere, input, modifierFlags, outputs);
    return handledHere;
}
