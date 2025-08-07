#include "GUIComponents/TextBox.hpp"

#include "Core/InputManager.hpp"
#include "Core/JSONLoader.hpp"
#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "Utility/Scaler.hpp"
#include "Utility/aligner.hpp"
#include <format>
TextBox::TextBox()
    : inputText(*ResourceManager::getInstance().getFont("pixel"), "", 30) {
    graphicsState.loadStyle(JSONLoader::getInstance().getStyle("basic_button"));
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition({0, 0});
    Aligner::align(inputText, HorizontalAlignment::Left,
                   VerticalAlignment::Middle);
}
TextBox& TextBox::setSize(sf::Vector2f size) {
    boundingBox.size = size;
    return *this;
}

TextBox& TextBox::setPosition(sf::Vector2f position) {
    boundingBox.position = position;
    return *this;
}

TextBox& TextBox::setText(const std::string& text) {
    inputText.setString(text);
    return *this;
}

TextBox& TextBox::clearText() {
    inputText.setString("");
    return *this;
}

TextBox& TextBox::setStyle(nlohmann::json styleConfig) {
    graphicsState.loadStyle(styleConfig);
    inputText.setFont(*graphicsState.getStyle().getFont());
    return *this;
}

TextBox& TextBox::adjust() {
    inputText = Aligner::align(inputText, HorizontalAlignment::Left,
                               VerticalAlignment::Middle);
    inputText.setPosition(
        {boundingBox.position.x + graphicsState.getStyle().getPadding()[0],
         boundingBox.position.y});

    return *this;
}

bool TextBox::onMouseEvent(Mouse button, UserEvent event,
                           const sf::Vector2f& worldPosition,
                           const sf::Vector2f& windowPosition) {
    sf::FloatRect bound = boundingBox;
    bound.position.y -= boundingBox.size.y / 2.f;
    if (event == UserEvent::Press && button == Mouse::Left) {
        if (!graphicsState.isPressed() &&
            bound.contains(windowPosition)) {
            graphicsState.updatePressState(true);
            subscribeTextInput(
                InputManager::getInstance().getTextInputProcessor());
            return true;
        } else {
            if (graphicsState.isPressed() &&
                !bound.contains(windowPosition)) {
                graphicsState.updatePressState(false);
                unsubscribeTextInput(
                    InputManager::getInstance().getTextInputProcessor());
            }
        }
        return false;
    }
    return false;
}

bool TextBox::onTextInput(int unicode) {
    if (unicode >= 32 && unicode <= 126) {  // ASCII printable characters
        std::string currentText = inputText.getString();
        currentText += static_cast<char>(unicode);
        sf::Text nextText(inputText.getFont());
        nextText.setString(currentText);
        nextText.setCharacterSize(inputText.getCharacterSize());
        if (maximumLength < 0) {
            Logger::debug(std::format("{} {} {}", nextText.getGlobalBounds().size.x, graphicsState.getStyle().getBorderWidth() * 2, boundingBox.size.x));
            if (nextText.getGlobalBounds().size.x +
                    graphicsState.getStyle().getPadding()[0] * 2 >=
                boundingBox.size.x)
                return false;
        }
        if (currentText.length() > maximumLength) 
            return false;
        inputText.setString(currentText);
        return true;
    } else if (unicode == 8) {  // Backspace
        std::string currentText = inputText.getString();
        if (!currentText.empty()) {
            currentText.pop_back();
            inputText.setString(currentText);
        }
        return true;
    }
    return false;
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::RectangleShape background(
        {boundingBox.size.x - 2 * graphicsState.getStyle().getBorderWidth(),
         boundingBox.size.y - 2 * graphicsState.getStyle().getBorderWidth()});
    background = Aligner::align(background, HorizontalAlignment::Left,
                                VerticalAlignment::Middle);
    background.setPosition(
        boundingBox.position +
        sf::Vector2f{graphicsState.getStyle().getBorderWidth(), 0});

    background.setFillColor(graphicsState.getFillColor());
    background.setOutlineColor(graphicsState.getBorderColor());
    background.setOutlineThickness(graphicsState.getStyle().getBorderWidth());

    target.draw(background, states);
    target.draw(inputText, states);
}

void TextBox::update() {
    graphicsState.update();
    inputText.setFillColor(graphicsState.getTextColor());
    adjust();
}

TextBox& TextBox::setMaximumLength(int length) {
    maximumLength = length;
    return *this;
}