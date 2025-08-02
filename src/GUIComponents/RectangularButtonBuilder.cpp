#include "GUIComponents/RectangularButtonBuilder.hpp"

#include "Core/ResourceManager.hpp"
#include "Utility/logger.hpp"
RectangularButtonBuilder::RectangularButtonBuilder(Mediator& mediator) : mediator{mediator} {}

RectangularButtonBuilder& RectangularButtonBuilder::setText(const std::string& text) {
    this->text = text;
    return *this;
}

RectangularButtonBuilder& RectangularButtonBuilder::setPosition(const sf::Vector2f position) {
    this->position = position;
    return *this;
}

RectangularButtonBuilder& RectangularButtonBuilder::setSize(const sf::Vector2f size) {
    this->size = size;
    return *this;
}

RectangularButtonBuilder& RectangularButtonBuilder::reset() {
    text = "";
    styleConfig = nlohmann::json();
    position = {0.f, 0.f};
    size = {0.f, 0.f};
    notificationMessage = "";
    callback = nullptr;
    tex = nullptr;
    return *this;
}

RectangularButtonBuilder& RectangularButtonBuilder::loadJson(std::string id) {
    styleConfig = JSONLoader::getInstance().getStyle(id);
    return *this;
}

std::unique_ptr<RectangularButton> RectangularButtonBuilder::build() {
    std::unique_ptr<RectangularButton> result(new RectangularButton(mediator));

    result->geometricInfo = {position, size};
    result->style.loadJson(styleConfig);
    result->setOnClick(callback);
    fontName = styleConfig["font"];

    if (tex) {
        std::unique_ptr<sf::Sprite> sprite = std::make_unique<sf::Sprite>(*tex);
        // sprite->setOrigin((sprite->getLocalBounds().position + sprite->getLocalBounds().size) / 2.f);
        sprite->setScale({size.x / sprite->getLocalBounds().size.x, size.y / sprite->getLocalBounds().size.y});
        sprite->setPosition(position);
        result->backgroundSprite = std::move(sprite);
    }
    std::unique_ptr<sf::Text> label = std::make_unique<sf::Text>(
        *ResourceManager::getInstance().getFont(fontName), text, 24);
    Logger::debug(std::format("{} {} {} {}", label->getLocalBounds().position.x,
                              label->getLocalBounds().position.y,
                              label->getLocalBounds().size.x,
                              label->getLocalBounds().size.y));
    label->setOrigin(label->getLocalBounds().position +
                     label->getLocalBounds().size / 2.f);
    label->setPosition(position + size / 2.f);
    result->label = std::move(label);
    return std::move(result);
}

RectangularButtonBuilder& RectangularButtonBuilder::setCallback(std::function<void(RectangularButton*)> call) {
    callback = call;
    return *this;
}

RectangularButtonBuilder& RectangularButtonBuilder::setBackground(const sf::Texture* tex) {
    this->tex = tex;
    return *this;
}