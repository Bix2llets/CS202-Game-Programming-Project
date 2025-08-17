#include "GUIComponents/CircularButton.hpp"

#include "Core/JSONLoader.hpp"
#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "Gameplay/RadialUpgradeMenu.hpp"
#include "Utility/aligner.hpp"
CircularButton::CircularButton()
    : ButtonBase(),
      displaySprite{*ResourceManager::getInstance().getTexture("sell_icon")} {
    graphicState.loadStyle(
        JSONLoader::getInstance().getStyle("upgrade_button"));
    buttonShape.setFillColor(sf::Color::White);
    buttonShape.setOutlineThickness(graphicState.getStyle().getBorderWidth());
    buttonShape.setOutlineColor(sf::Color::Black);

    buttonShape = Aligner::align(buttonShape, HorizontalAlignment::Center,
                                 VerticalAlignment::Middle);
    displaySprite = Aligner::align(displaySprite, HorizontalAlignment::Center,
                                   VerticalAlignment::Middle);
}
void CircularButton::update() {
    ButtonBase::update();
    sf::Color fillColor = graphicState.getFillColor();
    sf::Color borderColor = graphicState.getBorderColor();

    buttonShape.setFillColor(fillColor);
    buttonShape.setOutlineColor(borderColor);
    displaySprite.setColor(sf::Color(255, 255, 255, fillColor.a));
}

CircularButton& CircularButton::setPosition(const sf::Vector2f& position) {
    buttonShape.setPosition(position);
    displaySprite.setPosition(position);
    return *this;
}

CircularButton& CircularButton::setParentMediator(Mediator* radialMenu) {
    parentMediator = radialMenu;
    return *this;
}

void CircularButton::draw(sf::RenderTarget& target,
                          sf::RenderStates states) const {
    target.draw(buttonShape, states);
    target.draw(displaySprite, states);
}

bool CircularButton::onMouseEvent(Mouse button, UserEvent event,
                                  const sf::Vector2f& worldPosition,
                                  const sf::Vector2f& windowPosition) {
    if (!graphicState.isPressed() && event == UserEvent::Press &&
        contains(windowPosition) && button == Mouse::Left) {
        if (parentMediator) {
            onClickCallback(this);
            Logger::info("CircularButton: Sell button pressed");
        }
        graphicState.updatePressState(true);
        return true;
    }

    if (graphicState.isPressed() && event == UserEvent::Release &&
        button == Mouse::Left) {
        graphicState.updatePressState(false);
        return false;
    }
    if (!graphicState.isHovered() && event == UserEvent::Move &&
        contains(windowPosition)) {
        graphicState.updateHoverState(true);
        return false;
    }
    if (graphicState.isHovered() && event == UserEvent::Move &&
        !contains(windowPosition)) {
        graphicState.updateHoverState(false);
        return false;
    }
    return false;
}

bool CircularButton::onScrollEvent(float delta,
                                   const sf::Vector2f& worldPosition,
                                   const sf::Vector2f& windowPosition) {
    return false;
}
bool CircularButton::contains(const sf::Vector2f& mousePosition) {
    if ((mousePosition - buttonShape.getPosition()).length() <=
        buttonShape.getRadius() + buttonShape.getOutlineThickness()) {
        return true;
    }
    return false;
}

CircularButton& CircularButton::setRadius(float radius) {
    buttonShape.setRadius(radius);
    buttonShape.setOrigin({radius, radius});
    return *this;
}
CircularButton& CircularButton::setStyle(std::string style) {
    graphicState.loadStyle(JSONLoader::getInstance().getStyle(style));
    return *this;
}

CircularButton& CircularButton::setDisplaySprite(sf::Sprite sprite) {
    displaySprite = sprite;
    displaySprite = Aligner::align(displaySprite, HorizontalAlignment::Center,
                                   VerticalAlignment::Middle);
    displaySprite.setPosition(buttonShape.getPosition());
    return *this;
}

CircularButton& CircularButton::setCallback(
    std::function<void(CircularButton*)> callback) {
    onClickCallback = std::move(callback);
    return *this;
}