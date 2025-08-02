#pragma once

#include <SFML/Graphics.hpp>

class Tower;
class RadialUpgradeMenu {
    private:
    bool isUsing;
    sf::Vector2f position;
    sf::CircleShape ring;
    
    void setPosition(const sf::Vector2f &position);
    public:
        void show();
        void hide();
        void update();
        void render() const;

        void setFocusOnTower(Tower* tower);
};