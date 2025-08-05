#include "GUIComponents/GUIState.hpp"

#include "Utility/lerp.hpp"
#include "Core/JSONLoader.hpp"
GUIState::GUIState()
    : pressed(false), hovered(false) {
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
void GUIState::resetAnimation() {
    hover.setRemainingTime(hover.getInterval());
    press.setRemainingTime(press.getInterval());
    reverseHover.setRemainingTime(0);
    reversePress.setRemainingTime(0);
    pressed = false;
    hovered = false;
}

void GUIState::updatePressState(bool pressed) {
    if (pressed != this->pressed) {
        this->pressed = pressed;
        if (pressed) {
            press.reset();
            press.setRemainingTime(reversePress.getPassedTime());
            reversePress.reset();
        } else {
            reversePress.reset();
            reversePress.setRemainingTime(press.getPassedTime());
            press.reset();
        }
    }
}

void GUIState::updateHoverState(bool hovered) {
    if (hovered != this->hovered) {
        this->hovered = hovered;
        if (hovered) {
            hover.reset();
            hover.setRemainingTime(reverseHover.getPassedTime());
            reverseHover.reset();
        } else {
            reverseHover.reset();
            reverseHover.setRemainingTime(hover.getPassedTime());
            hover.reset();
        }
    }
}

void GUIState::update() {
    if (pressed) {
        press.update();
    } else {
        reversePress.update();
    }
    if (hovered) {
        hover.update();
    } else {
        reverseHover.update();
    }
}

sf::Color GUIState::getFillColor() const {
    sf::Color result = style.getNormal().background;
    if (hovered) {
        result = ColorMixer::perceptualLerp(result, style.getHover().background,
                                            hover.getCompletionPercentage());
    } else {
        result = ColorMixer::perceptualLerp(style.getHover().background, result,
                                            reverseHover.getCompletionPercentage());
    }

    if (pressed) {
        result = ColorMixer::perceptualLerp(result, style.getClick().background,
                                            press.getCompletionPercentage());
    } else {
        result = ColorMixer::perceptualLerp(style.getClick().background, result,
                                            reversePress.getCompletionPercentage());
    }

    return result;
}

sf::Color GUIState::getTextColor() const {
    sf::Color result = style.getNormal().text;
    if (hovered) {
        result = ColorMixer::perceptualLerp(result, style.getHover().text,
                                            hover.getCompletionPercentage());
    } else {
        result = ColorMixer::perceptualLerp(style.getHover().text, result,
                                            reverseHover.getCompletionPercentage());
    }

    if (pressed) {
        result = ColorMixer::perceptualLerp(result, style.getClick().text,
                                            press.getCompletionPercentage());
    } else {
        result = ColorMixer::perceptualLerp(style.getClick().text, result,
                                            reversePress.getCompletionPercentage());
    }

    return result;
}

sf::Color GUIState::getBorderColor() const {
    sf::Color result = style.getNormal().border;
    if (hovered) {
        result = ColorMixer::perceptualLerp(result, style.getHover().border,
                                            hover.getCompletionPercentage());
    } else {
        result = ColorMixer::perceptualLerp(style.getHover().border, result,
                                            reverseHover.getCompletionPercentage());
    }

    if (pressed) {
        result = ColorMixer::perceptualLerp(result, style.getClick().border,
                                            press.getCompletionPercentage());
    } else {
        result = ColorMixer::perceptualLerp(style.getClick().border, result,
                                            reversePress.getCompletionPercentage());
    }

    return result;
}

void GUIState::loadStyle(const nlohmann::json &file) {
    style.loadJson(file);
}

void GUIState::loadStyle(const std::string &ID) {
    nlohmann::json file = JSONLoader::getInstance().getStyle(ID);
    loadStyle(file);
}