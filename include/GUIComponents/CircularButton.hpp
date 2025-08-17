#pragma once

#include <functional>

#include "GUIComponents/ButtonBase.hpp"
#include "GUIComponents/mediator.hpp"
class RadialUpgradeMenu;
class CircularButton : public ButtonBase {
    public:
    CircularButton();
    ~CircularButton() override = default;
    CircularButton(const CircularButton&) = default;

    virtual CircularButton& setPosition(const sf::Vector2f& position);
    virtual CircularButton& setParentMediator(Mediator* mediator);
    virtual CircularButton& setRadius(float radius);
    virtual CircularButton& setStyle(std::string style);
    virtual CircularButton& setDisplaySprite(sf::Sprite sprite);
    virtual CircularButton& setCallback(
        std::function<void(CircularButton*)> callback);

    void update() override;
    sf::CircleShape buttonShape;

    // * Mouse position is the world position of the cursor
    bool contains(const sf::Vector2f& mousePosition) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool onMouseEvent(Mouse button, UserEvent event,
                      const sf::Vector2f& worldPosition,
                      const sf::Vector2f& windowPosition) override;
    bool onScrollEvent(float delta, const sf::Vector2f& worldPosition,
                       const sf::Vector2f& windowPosition) override;

    inline int getRadius() const { return buttonShape.getRadius(); }

    private:
    sf::Sprite displaySprite;
    Mediator* parentMediator = nullptr;
    std::function<void(CircularButton*)> onClickCallback;
    sf::Vector2f position;
};