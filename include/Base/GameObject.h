#pragma once
#include <SFML/Graphics.hpp>
#include "Base/Constants.h"
class GameObject : public sf::Drawable {
    virtual void draw(sf::RenderTarget &target, sf::RenderStates state) const = 0;
    virtual void update() const = 0;
    virtual ~GameObject() = default;
};