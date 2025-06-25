/**
 * @file InputManager.hpp
 * @brief Declares the InputManager class for handling input events.
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

#include "Core/MouseState.hpp"
/**
 * @class InputManager
 * @brief Handles input events and manages mouse state.
 */
class InputManager {
    MouseState mouseState; ///< Manages mouse button subscriptions and events.
    sf::RenderWindow &window; ///< Reference to the main window.
    public:
    /**
     * @brief Constructs an InputManager for the given window.
     * @param window Reference to the SFML render window.
     */
    InputManager(sf::RenderWindow &window);
    /**
     * @brief Handles an input event.
     * @param event Optional SFML event to handle.
     */
    void handleEvent(std::optional<sf::Event>& event);
    /**
     * @brief Returns a reference to the MouseState.
     * @return Reference to the MouseState object.
     */
    inline MouseState& getMouseState() { return mouseState; };
};