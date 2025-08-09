#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#include "Core/MouseObserver.hpp"
#include "Entity/Modules/EntityStat.hpp"
#include "Entity/Modules/Timer.hpp"
#include "Gameplay/Currency.hpp"
#include "Entity/Tower/Upgrades/UpgradeDetails.hpp"
class Tower;

// * VIsual only, does not interact with the logic of Level
class TowerInfoPanel {
    private:
    const Tower* referencingTower;

    public:
    TowerInfoPanel();
    TowerInfoPanel(const TowerInfoPanel& other) = delete;
    TowerInfoPanel& operator=(const TowerInfoPanel& other) = delete;

    private:
    sf::Text range;
    sf::Text damage;
    sf::Text fireRate;
    sf::Text maxTargets;

    sf::Sprite rangeIcon;
    sf::Sprite damageIcon;
    sf::Sprite fireRateIcon;
    sf::Sprite maxTargetsIcon;

    sf::RectangleShape background;

    sf::Sprite towerSprite;
    sf::Text towerName;
    sf::Text upgradeTitle;

    Timer previewClosingTimer;

    const UpgradeDetails* currentUpgradeDetail = nullptr;

    std::vector<sf::Text> upgradeContents;

    public:
    static TowerInfoPanel& getInstance();
    void render(sf::RenderStates state = sf::RenderStates::Default) const;
    void setFocus(Tower* tower);
    void deFocus();
    void update();

    void displayUpgrade(const UpgradeDetails* detail);
    void clearDisplayUpgrade();
    inline bool isDisplaying() const { return bool(referencingTower); };
};