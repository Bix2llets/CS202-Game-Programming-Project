#include "GUIComponents/RectangularButtonBuilder.hpp"

#include "Core/ResourceManager.hpp"
#include "Utility/logger.hpp"
RectangularButtonBuilder::RectangularButtonBuilder(Mediator& mediator) : mediator{mediator} {}

RectangularButtonBuilder& RectangularButtonBuilder::setText(const std::string& text) {
    this->text = text;
    return *this;
}

RectangularButtonBuilder& RectangularButtonBuilder::setPosition(const sf::Vector2f position) {
    buttonShape.setPosition(position);
    return *this;
}

RectangularButtonBuilder& RectangularButtonBuilder::setSize(const sf::Vector2f size) {
    buttonShape.setSize(size);
    return *this;
}

RectangularButtonBuilder& RectangularButtonBuilder::reset() {
    text = "";
    styleConfig = nlohmann::json();
    buttonShape = sf::RectangleShape();
    notificationMessage = "";
    callback = nullptr;
    tex = nullptr;
    textSize = 24;
    return *this;
}

RectangularButtonBuilder& RectangularButtonBuilder::loadJson(std::string id) {
    styleConfig = JSONLoader::getInstance().getStyle(id);
    return *this;
}

std::unique_ptr<RectangularButton> RectangularButtonBuilder::build() {
    std::unique_ptr<RectangularButton> result(new RectangularButton(mediator));

    result->graphicState.loadStyle(styleConfig);
    int borderWidth = result->graphicState.getStyle().getBorderWidth();
    result->rect = buttonShape;
    result->rect.setPosition(result->rect.getPosition() + sf::Vector2f{borderWidth, borderWidth});
    result->rect.setSize(result->rect.getSize() - sf::Vector2f(borderWidth * 2, borderWidth * 2));
    result->rect.setOutlineThickness(result->graphicState.getStyle().getBorderWidth());
    result->setOnClick(callback);
    fontName = styleConfig["font"];
    
    std::unique_ptr<sf::Text> label = std::make_unique<sf::Text>(
        *ResourceManager::getInstance().getFont(fontName), text, 24);
    Logger::debug(std::format("{} {} {} {}", label->getLocalBounds().position.x,
                              label->getLocalBounds().position.y,
                              label->getLocalBounds().size.x,
                              label->getLocalBounds().size.y));
    label->setOrigin(label->getLocalBounds().position +
                     label->getLocalBounds().size / 2.f);
    label->setPosition(result->rect.getGlobalBounds().position + result->rect.getGlobalBounds().size / 2.f);
    result->label = std::move(label);
    return std::move(result);
}

RectangularButtonBuilder& RectangularButtonBuilder::setCallback(std::function<void(RectangularButton*)> call) {
    callback = call;
    return *this;
}

RectangularButtonBuilder& RectangularButtonBuilder::setBackground(const sf::Texture* tex) {
    buttonShape.setTexture(tex);
    return *this;
}

RectangularButtonBuilder& RectangularButtonBuilder::setBackgroundViewport(sf::IntRect rect) {
    this->buttonShape.setTextureRect(rect);
    return *this;
}

RectangularButtonBuilder& RectangularButtonBuilder::setTextSize(int size) {
    textSize = size;
    return *this;
}