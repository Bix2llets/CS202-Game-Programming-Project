#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class Interative {
    virtual void handleInput(sf::RenderWindow &window) = 0;
    virtual void handleEvent(sf::RenderWindow &window,
                             std::optional<sf::Event> &event) = 0;
};