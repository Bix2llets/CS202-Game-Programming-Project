#pragma once

#include "GUIComponents/Mediator.hpp"

#include "GUIComponents/RectangularButton.hpp"
#include "Core/ResourceManager.hpp"
#include <memory>
// Do nothing on its own.
class Overlay : public Mediator {
    protected:
    sf::RectangleShape
        background;    ///< Background sprite for the game 
    sf::RectangleShape darkOverlay;
    sf::Text title;    ///< Title text for the game over screen
    std::vector<std::unique_ptr<RectangularButton>> buttons;
    Mediator& parentScene;

    public:
    Overlay(Mediator& mediator) : title(*ResourceManager::getInstance().getFont("pixel")), parentScene{mediator} {}
    virtual ~Overlay() = default;

    virtual void update() = 0;
    virtual void render() = 0;
};