#pragma once

#include <SFML/Graphics.hpp>
#include "Gameplay/Currency.hpp"
#include "Core/MouseObserver.hpp"
class Tower;

class TowerPanelButton : public sf::Drawable, public MouseObserver {
    sf::Vector2f position;
    sf::CircleShape background;
    sf::Sprite foreground;

    sf::RectangleShape priceTag;

    Currency petroleum, scrap;

    public:
    void render(sf::RenderStates state = sf::RenderStates::Default);

    void setForeground(sf::Texture texture);
    void setForeground(sf::Sprite sprite);


    
};
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