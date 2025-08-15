#pragma once

#include <SFML/Graphics.hpp>

#include "Core/MouseObserver.hpp"
#include "Entity/Modules/Timer.hpp"
#include "GUICOmponents/Style.hpp"
#include "GUIComponents/GUIState.hpp"
#include "Utility/lerp.hpp"
class ButtonBase : public sf::Drawable, public MouseObserver {
   protected:

    GUIState graphicState;
    virtual bool contains(const sf::Vector2f &position) = 0;
    
    public:
    ButtonBase();
    ~ButtonBase();
    virtual void draw(sf::RenderTarget &target,
                      sf::RenderStates state) const override = 0;
    virtual bool onMouseEvent(Mouse button, UserEvent event,
                              const sf::Vector2f &worldPosition,
                              const sf::Vector2f &windowPosition) = 0;
    virtual bool onScrollEvent(float delta,
                              const sf::Vector2f &worldPosition,
                              const sf::Vector2f &windowPosition) = 0;

    virtual void update();

    inline void resetAnimation() {
        graphicState.resetAnimation();
    }

    inline bool isHovered() {return graphicState.isHovered();}
    inline bool isPressed() {return graphicState.isPressed();} 


};