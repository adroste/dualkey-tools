#include "./OutputKeyCodeHandler.h"

bool OutputKeyCodeHandler::handleEvent(bool, const InputEvent& input, ModifierFlags, std::vector<InputEvent>& outputs)
{
    std::cout << "\rKeyCode: " << std::setw(4) << static_cast<int>(input.keyCode) << " << ";

    outputs.push_back(input);
    return true;
}
