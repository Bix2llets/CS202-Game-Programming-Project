#include "GUIComponents/RectangularButton.hpp"

#include <SFML/Graphics.hpp>
#include <format>

#include "Base/Constants.hpp"
#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "Utility/lerp.hpp"
RectangularButton::RectangularButton(Mediator& mediator) : mediator(mediator) {}

void RectangularButton::setOnClick(
    const std::function<void(RectangularButton*)>& callback) {
    onClick = callback;
}

void RectangularButton::click() {
    if (onClick) onClick(this);
}

void RectangularButton::draw(sf::RenderTarget& target,
                             sf::RenderStates states) const {
    sf::Color fillColor;
    sf::Color textColor;

    fillColor = graphicState.getFillColor();
    textColor = graphicState.getTextColor();

    target.draw(rect, states);
    // ? Uncomment these lines to see the bounding box for the text of button
    // sf::RectangleShape textBound;
    // textBound.setSize(label->getLocalBounds().size);
    // textBound.setPosition(label->getPosition());
    // textBound.setOrigin(textBound.getLocalBounds().position +
    // textBound.getLocalBounds().size / 2.f);
    // textBound.setFillColor(sf::Color::Black);
    // textBound.setOutlineColor(sf::Color::Green);
    // textBound.setOutlineThickness(1.f);
    // target.draw(textBound, states);
    label->setFillColor(textColor);
    target.draw(*label, states);
}

std::string RectangularButton::getLabel() const { return label->getString(); }

sf::Vector2f RectangularButton::getPosition() const {
    return rect.getGlobalBounds().position;
}

bool RectangularButton::onMouseEvent(Mouse button, UserEvent event,
                                     const sf::Vector2f& worldPosition,
                                     const sf::Vector2f& windowPosition) {
    if (button == Mouse::Left && event == UserEvent::Press)
        if (contains(static_cast<sf::Vector2f>(windowPosition))) {
            graphicState.updatePressState(true);
            click();

            return true;
        }

    if (button == Mouse::Left && event == UserEvent::Release) {
        graphicState.updatePressState(false);
        return false;
    }
    if (event == UserEvent::Move) {
        if (contains(static_cast<sf::Vector2f>(windowPosition))) {
            graphicState.updateHoverState(true);
            return true;
        } else {
            graphicState.updateHoverState(false);
            return false;
        }
    }
    return false;
}

void RectangularButton::subscribeMouseAll(MouseState& mouseState) {
    subscribeMouse(Mouse::Left, UserEvent::Press, mouseState);
    subscribeMouse(Mouse::None, UserEvent::Move, mouseState);
    subscribeMouse(Mouse::Left, UserEvent::Move, mouseState);
    subscribeMouse(Mouse::Right, UserEvent::Move, mouseState);
    subscribeMouse(Mouse::Middle, UserEvent::Move, mouseState);
    subscribeMouse(Mouse::Left, UserEvent::Release, mouseState);
}
void RectangularButton::unSubscribeMouseAll(MouseState& mouseState) {
    unSubscribeMouse(Mouse::Left, UserEvent::Press, mouseState);
    unSubscribeMouse(Mouse::None, UserEvent::Move, mouseState);
    unSubscribeMouse(Mouse::Left, UserEvent::Move, mouseState);
    unSubscribeMouse(Mouse::Right, UserEvent::Move, mouseState);
    unSubscribeMouse(Mouse::Left, UserEvent::Release, mouseState);
    unSubscribeMouse(Mouse::Middle, UserEvent::Move, mouseState);
}

void RectangularButton::update() {
    ButtonBase::update();
    rect.setFillColor(graphicState.getFillColor());
    rect.setOutlineColor(graphicState.getBorderColor());
    label->setFillColor(graphicState.getFillColor());
}

void RectangularButton::setDisplayText(std::string text) {
    label->setString(text);
    label->setOrigin(label->getLocalBounds().position +
                     label->getLocalBounds().size / 2.f);
    label->setPosition(geometricInfo.position + geometricInfo.size / 2.f);
}

void RectangularButton::resetAnimation() { graphicState.resetAnimation(); }

bool RectangularButton::onScrollEvent(float delta,
                                      const sf::Vector2f& worldPosition,
                                      const sf::Vector2f& windowPosition) {
    return false;
}

bool RectangularButton::contains(const sf::Vector2f& windowPosition) {
    return rect.getGlobalBounds().contains(windowPosition);
}

sf::Sprite RectangularButton::getBackground() {
    if (rect.getTexture() == nullptr)
        return sf::Sprite(GameConstants::BLANK_TEXTURE);
    return sf::Sprite(*rect.getTexture(), rect.getTextureRect());
}

void RectangularButton::setBackground(const sf::Texture* tex) {
    rect.setTexture(tex);
}

sf::Vector2f RectangularButton::getSize() const {
    return rect.getSize();
}