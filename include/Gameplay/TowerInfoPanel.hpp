#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#include "Core/MouseObserver.hpp"
#include "Entity/Modules/EntityStat.hpp"
#include "Entity/Modules/Timer.hpp"
#include "Entity/Tower/Upgrades/UpgradeDetails.hpp"
#include "GUICOmponents/Mediator.hpp"
#include "GUIComponents/RectangularButton.hpp"
#include "Gameplay/Currency.hpp"
class Level;
class Tower;

// * VIsual only, does not interact with the logic of Level
class TowerInfoPanel : public MouseObserver, public Mediator {
    private:
    Tower* referencingTower;


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
    sf::Sprite evolutionSprite;
    std::unique_ptr<Tower> evolutionPreviewTower = nullptr;
    sf::Text towerName;
    sf::Text upgradeTitle;

    std::unique_ptr<RectangularButton> prevModeButton, nextModeButton;
    Timer previewClosingTimer;

    const UpgradeDetails* currentUpgradeDetail = nullptr;

    std::vector<sf::Text> upgradeContents;
    sf::Text evolutionContents;
    public:
    static TowerInfoPanel& getInstance();
    void render(sf::RenderStates state = sf::RenderStates::Default) const;
    void setFocus(Tower* tower);
    void deFocus();
    void update();

    void displayUpgrade(const UpgradeDetails* detail);
    void displayEvolution(const std::string& evolveTo, Level* level);
    void clearDisplayUpgrade();
    inline bool isDisplaying() const { return bool(referencingTower); };

    bool onMouseEvent(Mouse button, UserEvent event,
                      const sf::Vector2f& worldPosition,
                      const sf::Vector2f& windowPosition) override;

    bool onScrollEvent(float delta, const sf::Vector2f& worldPosition,
                       const sf::Vector2f& windowPosition) override;
};