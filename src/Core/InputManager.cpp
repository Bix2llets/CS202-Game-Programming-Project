#include "Core/InputManager.hpp"

void InputManager::handleEvent(std::optional<sf::Event> &event) {
    mouseState.handleEvent(event);
    keyboardState.handleEvent(event);
}

InputManager::InputManager(sf::RenderWindow &window)
    : window{window}, mouseState{window}, keyboardState{window} {}