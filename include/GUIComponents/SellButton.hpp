#pragma once

#include "GUIComponents/ButtonBase.hpp"
class RadialUpgradeMenu;
class SellButton : public ButtonBase {
    public:
    SellButton();
    ~SellButton() override = default;
    SellButton(const SellButton&) = default;

    SellButton& setPosition(const sf::Vector2f& position);
    SellButton& setParentRadialMenu(RadialUpgradeMenu* radialMenu);

    
    void update() override;
    sf::CircleShape buttonShape;

    // * Mouse position is the world position of the cursor
    bool contains(const sf::Vector2f& mousePosition) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool onMouseEvent(Mouse button, UserEvent event, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) override;
    bool onScrollEvent(float delta, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) override;

    private:
    sf::Sprite sellIcon;
    RadialUpgradeMenu* parentRadialMenu = nullptr;
    sf::Vector2f position;
};