#pragma once

#include <SFML/Graphics.hpp>

#include "Core/MouseObserver.hpp"
#include "Entity/Modules/Timer.hpp"
#include "GUICOmponents/Style.hpp"
#include "Utility/lerp.hpp"
class ButtonBase : public sf::Drawable, public MouseObserver {
   protected:
    sf::Vector2f position;  // Could be the center or the top-left cornder,
                            // depending on the implementation

    Style style;
    Timer hover;
    Timer press;
    Timer reverseHover;
    Timer reversePress;

    bool isPressed;
    bool isHovered;

    virtual bool contains(const sf::Vector2f &position) = 0;
    
    void updatePressState(bool isPressed);
    void updateHoverState(bool isPressed);
    
    virtual sf::Color getFillColor() const;
    virtual sf::Color getTextColor() const;
    virtual sf::Color getBorderColor() const;
    public:
    ButtonBase();
    ~ButtonBase();
    void resetAnimation();
    virtual void draw(sf::RenderTarget &target,
                      sf::RenderStates state) const override = 0;
    virtual bool onMouseEvent(Mouse button, UserEvent event,
                              const sf::Vector2f &worldPosition,
                              const sf::Vector2f &windowPosition) = 0;
    virtual bool onScrollEvent(float delta,
                              const sf::Vector2f &worldPosition,
                              const sf::Vector2f &windowPosition) = 0;

    virtual void update();

};