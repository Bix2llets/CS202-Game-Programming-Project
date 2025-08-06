#pragma once

#include <SFML/Graphics.hpp>

#include "Core/MouseObserver.hpp"
#include "Entity/Tower/Upgrades/UpgradeDetails.hpp"
#include "GUIComponents/Mediator.hpp"
#include "GUIComponents/UpgradeButton.hpp"
#include "GUIComponents/SellButton.hpp"
class Tower;
class Level;
class UpgradeManager;
class RadialUpgradeMenu : public Mediator, public MouseObserver {
    private:
    bool displaying;
    sf::Vector2f position;
    sf::CircleShape ring;

    std::vector<UpgradeButton> upgradeButtons;
    SellButton sellBtn;
    Level& parentLevel;
    UpgradeManager* upgradeManager = nullptr;
    Tower* refTower = nullptr;

    void updatePositions();

    public:
    RadialUpgradeMenu(Level& parentLevel);

    const UpgradeDetails* getUpgradeDetail(int id);

    void setFocus(Tower* tower);
    void removeFocus(); 

    inline bool isDisplaying() const {return displaying;}

    bool onMouseEvent(Mouse button, UserEvent event,
                                 const sf::Vector2f& worldPosition,
                                 const sf::Vector2f& windowPosition) override;
    bool onScrollEvent(float delta, const sf::Vector2f& worldPosition, const sf::Vector2f& windowPosition);

    void render(sf::RenderStates state = sf::RenderStates::Default) const;

    void update();


};