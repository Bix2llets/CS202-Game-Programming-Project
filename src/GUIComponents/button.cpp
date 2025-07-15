#include "GUIComponents/button.hpp"

#include <SFML/Graphics.hpp>

#include "Base/Constants.hpp"
#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "Utility/lerp.hpp"
Button::Button(sf::FloatRect geometricInfo, Mediator& mediator)
    : geometricInfo(geometricInfo),
      mediator(mediator),
      onClickMessage("ButtonClicked"),
      isPressed{false},
      isHovered{false} {
    hover.setTimeInterval(0.5)
        .setTimerDirection(TimerDirection::Forward)
        .setTimerMode(TimerMode::Single)
        .setRemainingTime(0.5);
    press.setTimeInterval(0.5)
        .setTimerDirection(TimerDirection::Forward)
        .setTimerMode(TimerMode::Single)
        .setRemainingTime(0.5);

    reverseHover.setTimeInterval(0.5)
        .setTimerDirection(TimerDirection::Backward)
        .setTimerMode(TimerMode::Single)
        .setRemainingTime(0);

    reversePress.setTimeInterval(0.5)
        .setTimerDirection(TimerDirection::Backward)
        .setTimerMode(TimerMode::Single)
        .setRemainingTime(0);
}

void Button::setOnClick(const std::function<void(Button*)>& callback) {
    onClick = callback;
}

void Button::click() {
    mediator.notify(onClickMessage, this);
    if (onClick) onClick(this);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::RectangleShape rect;
    rect.setPosition(geometricInfo.position);
    rect.setSize(geometricInfo.size);
    sf::Color fillColor;
    sf::Color textColor;

    fillColor = style.getNormal().background;
    textColor = style.getNormal().text;
    if (isHovered) {
        fillColor =
            ColorMixer::perceptualLerp(fillColor, style.getHover().background,
                                       hover.getCompletionPercentage());
        textColor = ColorMixer::perceptualLerp(textColor, style.getHover().text,
                                               hover.getCompletionPercentage());
    } else {
        fillColor =
            ColorMixer::perceptualLerp(style.getHover().background, fillColor,
                                       reverseHover.getCompletionPercentage());
        textColor =
            ColorMixer::perceptualLerp(style.getHover().text, fillColor,
                                       reverseHover.getCompletionPercentage());
    }

    if (isPressed) {
        fillColor =
            ColorMixer::perceptualLerp(fillColor, style.getClick().background,
                                       press.getCompletionPercentage());
        textColor = ColorMixer::perceptualLerp(textColor, style.getClick().text,
                                               press.getCompletionPercentage());
    } else {
        fillColor =
            ColorMixer::perceptualLerp(style.getClick().background, fillColor,
                                       reversePress.getCompletionPercentage());
        textColor =
            ColorMixer::perceptualLerp(style.getClick().text, textColor,
                                       reversePress.getCompletionPercentage());
    }

    rect.setFillColor(fillColor);

    target.draw(rect, states);
    // ? Uncomment these lines to see the bounding box for the text of button
    // sf::RectangleShape textBound;
    // textBound.setSize(label->getLocalBounds().size);
    // textBound.setPosition(label->getPosition());
    // textBound.setOrigin(textBound.getLocalBounds().position + textBound.getLocalBounds().size / 2.f);
    // textBound.setFillColor(sf::Color::Black);
    // textBound.setOutlineColor(sf::Color::Green);
    // textBound.setOutlineThickness(1.f);
    // target.draw(textBound, states);
    target.draw(*label, states);
}

std::string Button::getLabel() const { return label->getString(); }

sf::Vector2f Button::getPosition() const { return geometricInfo.position; }

void Button::onMouseEvent(Mouse button, UserEvent event,
                          const sf::Vector2f& worldPosition,
                          const sf::Vector2f& windowPosition) {
    if (button == Mouse::Left && event == UserEvent::Press)
        if (geometricInfo.contains(static_cast<sf::Vector2f>(windowPosition))) {
            click();
            if (isPressed == true) return;
            isPressed = true;
            press.reset();
            press.setRemainingTime(reversePress.getPassedTime());
            reversePress.reset();

            return;
        }

    if (button == Mouse::Left && event == UserEvent::Release) {
        if (!isPressed) return;
        isPressed = false;
        reversePress.reset();
        reversePress.setRemainingTime(press.getPassedTime());
        press.reset();
    }
    if (event == UserEvent::Move) {
        if (geometricInfo.contains(static_cast<sf::Vector2f>(windowPosition))) {
            if (isHovered == true) return;
            isHovered = true;
            hover.reset();
            hover.setRemainingTime(reverseHover.getPassedTime());
            reverseHover.reset();
        } else {
            if (!(isHovered)) return;
            isHovered = false;

            reverseHover.reset();
            Logger::debug(std::format("{} {}", hover.getPassedTime(),
                                      press.getPassedTime()));

            reverseHover.setRemainingTime(hover.getPassedTime());

            hover.reset();
        }
    }
}

void Button::setNotificationMessage(const std::string& str) {
    onClickMessage = str;
}

void Button::subscribeMouseAll(MouseState& mouseState) {
    subscribeMouse(Mouse::Left, UserEvent::Press, mouseState);
    subscribeMouse(Mouse::None, UserEvent::Move, mouseState);
    subscribeMouse(Mouse::Left, UserEvent::Move, mouseState);
    subscribeMouse(Mouse::Right, UserEvent::Move, mouseState);
    subscribeMouse(Mouse::Left, UserEvent::Release, mouseState);
}
void Button::unSubscribeMouseAll(MouseState& mouseState) {
    unSubscribeMouse(Mouse::Left, UserEvent::Press, mouseState);
    unSubscribeMouse(Mouse::None, UserEvent::Move, mouseState);
    unSubscribeMouse(Mouse::Left, UserEvent::Move, mouseState);
    unSubscribeMouse(Mouse::Right, UserEvent::Move, mouseState);
    unSubscribeMouse(Mouse::Left, UserEvent::Release, mouseState);
}

void Button::update() {
    if (isPressed) {
        press.update();
    } else {
        reversePress.update();
    }
    if (isHovered) {
        hover.update();
    } else {
        reverseHover.update();
    }
}

void Button::setDisplayText(std::string text) {
    label->setString(text);
    label->setOrigin(label->getLocalBounds().position +
                     label->getLocalBounds().size / 2.f);
    label->setPosition(geometricInfo.position + geometricInfo.size / 2.f);
}