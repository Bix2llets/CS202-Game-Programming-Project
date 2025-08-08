#pragma once

#include <SFML/Graphics.hpp>

#include "Core/MouseObserver.hpp"
#include "Entity/Tower/Tower.hpp"
class TowerDisplayMenu : public sf::Drawable, public MouseObserver {
    private:
    Tower* refTower;

    public:
    void setRefTower(Tower* tower);
    void clearRefTower();

    bool onMouseEvent(Mouse mouse, UserEvent event, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) override;
    bool onScrollEvent(float delta, const sf::Vector2f &worldPosition, const sf::Vector2f& windowPosition) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};