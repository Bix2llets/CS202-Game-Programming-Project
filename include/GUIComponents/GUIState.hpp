#pragma once

#include <SFML/Graphics.hpp>

#include "GUIComponents/Style.hpp"
#include "Entity/Modules/Timer.hpp"
class GUIState {
    private:
    Style style;
    Timer hover;
    Timer press;
    Timer reverseHover;
    Timer reversePress;

    bool pressed;
    bool hovered;

    sf::Color overlayColor;
    public:
    GUIState();

    void resetAnimation();
    void updatePressState(bool isPressed);
    void updateHoverState(bool isPressed);
    void update();

    sf::Color getFillColor() const;
    sf::Color getTextColor() const;
    sf::Color getBorderColor() const;
    inline const Style& getStyle() const { return style;}

    inline bool isPressed() const { return pressed; }
    inline bool isHovered() const { return hovered; }

    void loadStyle(const nlohmann::json &file);
    void loadStyle(const std::string &ID);

    void setOverlayColor(const sf::Color &color);
    void removeOverlayColor();
};
