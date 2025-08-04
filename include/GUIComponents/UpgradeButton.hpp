#pragma once

#include <SFML/Graphics.hpp>

#include "Core/MouseObserver.hpp"
#include "GUIComponents/ButtonBase.hpp"
#include "Gameplay/Currency.hpp"
class RadialUpgradeMenu;
class UpgradeButton : public ButtonBase {
    public:
    UpgradeButton();
    ~UpgradeButton() override = default;
    UpgradeButton(const UpgradeButton&);
    UpgradeButton& setPosition(const sf::Vector2f& position);
    UpgradeButton& setRadius(int radius);
    UpgradeButton& setPrice(Currency price);
    UpgradeButton& setParentRadialMenu(RadialUpgradeMenu* radialMenu);
    UpgradeButton& setUpgradeID(int id);
    
    void update() override;
    
    void refreshInfo();
    void setUpgradePossibility(bool canUpgrade);
    
    // * Mouse position is the world position of the cursor
    bool contains(const sf::Vector2f& mousePosition) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    bool onMouseEvent(Mouse button, UserEvent event, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) override;
    bool onScrollEvent(float delta, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) override;

    void setCanUpgrade(bool canUpgrade);
    private:
    void updatePriceTag();
    int radius;
    
    Currency price;
    sf::CircleShape buttonShape;
    sf::RenderTexture priceTag;
    sf::Sprite tagDisplay;
    sf::Sprite upgradeIcon;
    RadialUpgradeMenu* parentRadialMenu;
    bool canUpgrade = false;

    int upgradeID;
};