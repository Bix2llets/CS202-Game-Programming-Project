#pragma once

#include <SFML/Graphics.hpp>
#include "Gameplay/Currency.hpp"
class Tower;

class TowerPanelButton {
    sf::Vector2f position;
    sf::CircleShape background;
    sf::Sprite foreground;

    sf::RectangleShape priceTag;

    Currency petroleum, scrap;

    public:
    void render(sf::RenderStates state = sf::RenderStates::Default);

    
}
class TowerPanel {
    private:
    Tower* referencingTower;
    sf::CircleShape radialMenu;
    const int radialRadius = 15;

    TowerPanel();
    TowerPanel(const TowerPanel &other) = delete;
    TowerPanel& operator=(const TowerPanel &other) = delete;
    public:
    TowerPanel& getInstance();
    void render(sf::RenderStates state = sf::RenderStates::Default);

    

};