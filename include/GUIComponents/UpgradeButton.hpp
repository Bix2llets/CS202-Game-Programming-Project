#pragma once

#include <SFML/Graphics.hpp>

#include "Core/MouseObserver.hpp"
#include "GUIComponents/ButtonBase.hpp"

#include "Gameplay/Currency.hpp"
class RadialUpgradeMenu;
class UpgradeButton : public ButtonBase,
                      public sf::Drawable,
                      public MouseObserver {
    public:
    UpgradeButton();
    ~UpgradeButton() override = default;
    UpgradeButton& setPosition(const sf::Vector2f& position);
    UpgradeButton& setRadius(int radius);
    UpgradeButton& render(sf::RenderWindow& window) const;
    UpgradeButton& isHovered(const sf::Vector2f& mousePosition) const;
    UpgradeButton& setPetroleumPrice(int price);
    UpgradeButton& setScrapPrice(int price);
    UpgradeButton& setUpgradeIcon(sf::Sprite icon);
    UpgradeButton& setParentRadialMenu(RadialUpgradeMenu* radialMenu);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void update() override;

    
    private:
    void updatePriceTag();
    int radius;
    
    Currency price;
    sf::CircleShape buttonShape;
    sf::RenderTexture priceTag;
    sf::Sprite tagDisplay;

    sf::Sprite upgradeIcon;


    RadialUpgradeMenu* parentRadialMenu;
};