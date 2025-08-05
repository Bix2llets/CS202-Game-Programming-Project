#pragma once

#include <SFML/Graphics.hpp>

#include "Core/MouseObserver.hpp"
#include "GUIComponents/ButtonBase.hpp"
#include "Gameplay/Currency.hpp"
#include "Entity/Tower/Upgrades/UpgradeManager.hpp"
class RadialUpgradeMenu;
class UpgradeButton : public ButtonBase {
    public:
    UpgradeButton();
    ~UpgradeButton() override = default;
    UpgradeButton(const UpgradeButton&);
    UpgradeButton& setPosition(const sf::Vector2f& position);
    UpgradeButton& setRadius(int radius);
    UpgradeButton& setParentRadialMenu(RadialUpgradeMenu* radialMenu);
    UpgradeButton& setUpgradeManager(UpgradeManager &target);
    UpgradeButton& setUpgradeID(int id);
    UpgradeButton& setCanUpgrade(bool val);
    void update() override;
    
    void refreshInfo();
    
    // * Mouse position is the world position of the cursor
    bool contains(const sf::Vector2f& mousePosition) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    bool onMouseEvent(Mouse button, UserEvent event, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) override;
    bool onScrollEvent(float delta, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) override;

    private:

    const float TAG_OFFSET = 0.85f; 
    void updatePriceTag();
    void updateSpritePosition();
    int radius;
    
    Currency price;
    sf::CircleShape buttonShape;
    sf::RenderTexture priceTag;
    sf::Sprite tagDisplay;
    sf::Sprite upgradeIcon;

    RadialUpgradeMenu* parentRadialMenu;
    bool canUpgrade = false;
    bool isCapped = false;
    int upgradeID;

    UpgradeManager* upgrades;

    sf::Color getFillColor();
    sf::Color getBorderColor();
    sf::Color getTextColor();
};