#include "Core/InputManager.hpp"

void InputManager::handleEvent(std::optional<sf::Event> &event) {
    mouseState.handleEvent(event);
    keyboardState.handleEvent(event);
    textInputProcessor.handleEvent(event);
}

InputManager::InputManager() {} 

InputManager& InputManager::getInstance() {
    static InputManager instance;
    return instance;
}
