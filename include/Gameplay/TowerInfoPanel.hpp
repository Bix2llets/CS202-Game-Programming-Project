#pragma once

#include <SFML/Graphics.hpp>
#include "Gameplay/Currency.hpp"
#include "Core/MouseObserver.hpp"
#include "Entity/Modules/EntityStat.hpp"
class Tower;

// * VIsual only, does not interact with the logic of Level
class TowerInfoPanel {
    private:
    const Tower* referencingTower;

    TowerInfoPanel();
    TowerInfoPanel(const TowerInfoPanel &other) = delete;
    TowerInfoPanel& operator=(const TowerInfoPanel &other) = delete;

    sf::Text range;
    sf::Text damage;
    sf::Text fireRate;
    sf::Text maxTargets;

    sf::Sprite rangeIcon;
    sf::Sprite damageIcon;
    sf::Sprite fireRateIcon;
    sf::Sprite maxTargetsIcon;

    sf::RectangleShape background;
    
    sf::Text towerName;
    public:
    static TowerInfoPanel& getInstance();
    void render(sf::RenderStates state = sf::RenderStates::Default);
    void setFocus(Tower* tower);
    void deFocus();
    void update();
    inline bool isDisplaying() const {return bool(referencingTower);};
    

};