#pragma once

#include <SFML/Graphics.hpp>
#include "Gameplay/Currency.hpp"
#include "Core/MouseObserver.hpp"
class Tower;

// * VIsual only, does not interact with the logic of Level
class TowerInfoPanel {
    private:
    Tower* referencingTower;

    TowerInfoPanel();
    TowerInfoPanel(const TowerInfoPanel &other) = delete;
    TowerInfoPanel& operator=(const TowerInfoPanel &other) = delete;
    public:
    TowerInfoPanel& getInstance();
    void render(sf::RenderStates state = sf::RenderStates::Default);
    void setFocus(Tower* tower);
    void deFocus();
    void update();
    inline bool isDisplaying() const {return bool(referencingTower);};
    

};