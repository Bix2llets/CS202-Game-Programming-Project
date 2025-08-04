#include "GUIComponents/ButtonBase.hpp"

#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"

ButtonBase::ButtonBase() : position(0.f, 0.f), isPressed(false), isHovered(false) {
    hover.setTimeInterval(0.2f)
        .setTimerMode(TimerMode::Single)
        .setRemainingTime(0.2f);
    press.setTimeInterval(0.2f)
        .setTimerMode(TimerMode::Single)
        .setRemainingTime(0.2f);

    reverseHover.setTimeInterval(0.2f)
        .setTimerMode(TimerMode::Single)
        .setRemainingTime(0.f);

    reversePress.setTimeInterval(0.2f)
        .setTimerMode(TimerMode::Single)
        .setRemainingTime(0.f);
}

ButtonBase::~ButtonBase() {
    // Destructor implementation if needed
}

void ButtonBase::resetAnimation() {
    hover.setRemainingTime(hover.getInterval());
    press.setRemainingTime(press.getInterval());
    reverseHover.setRemainingTime(0);
    reversePress.setRemainingTime(0);
    isPressed = false;
    isHovered = false;
}

void ButtonBase::updatePressState(bool isPressed) {
    if (isPressed != this->isPressed) {
        this->isPressed = isPressed;
        if (isPressed) {
            // Logger::debug("Button pressed");
            press.reset();
            press.setRemainingTime(reversePress.getPassedTime());
            reversePress.reset();
        } else {
            // Logger::debug("Button released");
            reversePress.reset();
            reversePress.setRemainingTime(press.getPassedTime());
            press.reset();
        }
    }
}

void ButtonBase::updateHoverState(bool isHovered) {
    if (isHovered != this->isHovered) {
        this->isHovered = isHovered;
        if (isHovered) {
            // Logger::debug("Button hovered");
            hover.reset();
            hover.setRemainingTime(reverseHover.getPassedTime());
            reverseHover.reset();
        } else {
            // Logger::debug("Button unhovered");
            reverseHover.reset();
            reverseHover.setRemainingTime(hover.getPassedTime());
            hover.reset();
        }
    }
}

void ButtonBase::update() {
    if (isPressed) {
        press.update();
        // Logger::debug(std::format("Button is pressed, updating press timer {}", press.getCompletionPercentage()));
    } else {
        reversePress.update();
        // Logger::debug("Button is unpressed, updating reversePress timer");
    }
    if (isHovered) {
        // Logger::debug(std::format("Button is hovered, updating hover timer {}", hover.getCompletionPercentage()));
        hover.update();
    } else {
        // Logger::debug("Button is unhovered, updating unhover timer");
        reverseHover.update();
    }
}

sf::Color ButtonBase::getFillColor() const {
    sf::Color result = style.getNormal().background;
    if (isHovered) {
        result = ColorMixer::perceptualLerp(result, style.getHover().background,
                                          hover.getCompletionPercentage());
    } else {
        result = ColorMixer::perceptualLerp(style.getHover().background, result,
                                          reverseHover.getCompletionPercentage());
    }

    if (isPressed) {
        result = ColorMixer::perceptualLerp(result, style.getClick().background,
                                          press.getCompletionPercentage());
    } else {
        result = ColorMixer::perceptualLerp(style.getClick().background, result,
                                          reversePress.getCompletionPercentage());
    }

    return result;
}

sf::Color ButtonBase::getTextColor() const {
    sf::Color result = style.getNormal().text;
    if (isHovered) {
        result = ColorMixer::perceptualLerp(result, style.getHover().text,
                                          hover.getCompletionPercentage());
    } else {
        result = ColorMixer::perceptualLerp(style.getHover().text, result,
                                          reverseHover.getCompletionPercentage());
    }

    if (isPressed) {
        result = ColorMixer::perceptualLerp(result, style.getClick().text,
                                          press.getCompletionPercentage());
    } else {
        result = ColorMixer::perceptualLerp(style.getClick().text, result,
                                          reversePress.getCompletionPercentage());
    }

    return result;
}

sf::Color ButtonBase::getBorderColor() const {
    sf::Color result = style.getNormal().border;
    if (isHovered) {
        result = ColorMixer::perceptualLerp(result, style.getHover().border,
                                          hover.getCompletionPercentage());
    } else {
        result = ColorMixer::perceptualLerp(style.getHover().border, result,
                                          reverseHover.getCompletionPercentage());
    }

    if (isPressed) {
        result = ColorMixer::perceptualLerp(result, style.getClick().border,
                                          press.getCompletionPercentage());
    } else {
        result = ColorMixer::perceptualLerp(style.getClick().border, result,
                                          reversePress.getCompletionPercentage());
    }

    return result;
}