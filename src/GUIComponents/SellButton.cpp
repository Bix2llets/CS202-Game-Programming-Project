#include "GUIComponents/SellButton.hpp"

#include "Core/JSONLoader.hpp"
#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "Gameplay/RadialUpgradeMenu.hpp"
#include "Utility/aligner.hpp"
SellButton::SellButton()
    : ButtonBase(),
      sellIcon{*ResourceManager::getInstance().getTexture("sell_icon")} {
    graphicState.loadStyle(JSONLoader::getInstance().getStyle("upgrade_button"));
    buttonShape.setFillColor(sf::Color::White);
    buttonShape.setOutlineThickness(4);
    buttonShape.setOutlineColor(sf::Color::Black);

    buttonShape.setRadius(35.f);
    buttonShape = Aligner::align(buttonShape, HorizontalAlignment::Center,
                   VerticalAlignment::Middle);
    sellIcon = Aligner::align(sellIcon, HorizontalAlignment::Center,
                   VerticalAlignment::Middle);
}
void SellButton::update() {
    ButtonBase::update();
    sf::Color fillColor = graphicState.getFillColor();
    sf::Color borderColor = graphicState.getBorderColor();

    buttonShape.setFillColor(fillColor);
    buttonShape.setOutlineColor(borderColor);
    sellIcon.setColor(sf::Color(255, 255, 255, fillColor.a));
}

SellButton& SellButton::setPosition(const sf::Vector2f& position) {
    buttonShape.setPosition(position);
    sellIcon.setPosition(position);
    return *this;
}

SellButton& SellButton::setParentRadialMenu(RadialUpgradeMenu* radialMenu) {
    parentRadialMenu = radialMenu;
    return *this;
}

void SellButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(buttonShape, states);
    target.draw(sellIcon, states);
}

bool SellButton::onMouseEvent(Mouse button, UserEvent event,
                              const sf::Vector2f& worldPosition,
                              const sf::Vector2f& windowPosition) {
    if (!graphicState.isPressed() && event == UserEvent::Press && contains(windowPosition) &&
        button == Mouse::Left) {
        if (parentRadialMenu) {
            parentRadialMenu->notify("sell");
            Logger::info("SellButton: Sell button pressed");
        }
        graphicState.updatePressState(true);
        return true;
    }

    if (graphicState.isPressed() && event == UserEvent::Release && button == Mouse::Left) {
        graphicState.updatePressState(false);
        return false;
    }
    if (!graphicState.isHovered() && event == UserEvent::Move && contains(windowPosition)) {
        graphicState.updateHoverState(true);
        return false;
    }
    if (graphicState.isHovered() && event == UserEvent::Move && !contains(windowPosition)) {
        graphicState.updateHoverState(false);
        return false;
    }
    return false;
}

bool SellButton::onScrollEvent(float delta, const sf::Vector2f& worldPosition,
                   const sf::Vector2f& windowPosition) {
    return false;
}
bool SellButton::contains(const sf::Vector2f& mousePosition) {
    if ((mousePosition - buttonShape.getPosition()).length() <= buttonShape.getRadius() + buttonShape.getOutlineThickness()) {
        return true;
    }
    return false;
}