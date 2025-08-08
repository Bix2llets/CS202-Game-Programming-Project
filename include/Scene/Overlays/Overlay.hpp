#pragma once

#include "GUIComponents/Mediator.hpp"

#include "GUIComponents/RectangularButton.hpp"
#include "Core/ResourceManager.hpp"
// Do nothing on its own.
class Overlay : public Mediator {
    protected:
    sf::RectangleShape
        background;    ///< Background sprite for the game over screen
    sf::RectangleShape darkOverlay;
    sf::Text title;    ///< Title text for the game over screen
    sf::Text message;  ///< Message text for the game over screen
    std::vector<RectangularButton> buttons;

    public:
    Overlay() : title(*ResourceManager::getInstance().getFont("pixel")),
                message(*ResourceManager::getInstance().getFont("pixel")) {}
    virtual ~Overlay() = default;

    virtual void update() = 0;
    virtual void render() = 0;
};