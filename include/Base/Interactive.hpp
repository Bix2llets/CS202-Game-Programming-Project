/**
 * @file Interactive.hpp
 * @brief Declares the Interactive interface for objects that handle input and events.
 */
#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

/**
 * @class Interative
 * @brief Interface for objects that can handle input and events.
 */
class Interative {
    /**
     * @brief Handle input from the window (e.g., polling real-time input).
     * @param window Reference to the render window.
     */
    virtual void handleInput(sf::RenderWindow &window) = 0;
    /**
     * @brief Handle a specific event from the window.
     * @param window Reference to the render window.
     * @param event Optional SFML event to handle.
     */
    virtual void handleEvent(sf::RenderWindow &window,
                             std::optional<sf::Event> &event) = 0;
};