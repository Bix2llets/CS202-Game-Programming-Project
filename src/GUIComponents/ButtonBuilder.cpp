#include "GUIComponents/ButtonBuilder.hpp"

#include "Core/ResourceManager.hpp"
#include "Utility/logger.hpp"
ButtonBuilder::ButtonBuilder(Mediator& mediator, ResourceManager& resManager)
    : mediator{mediator}, resManager{resManager} {}

ButtonBuilder& ButtonBuilder::setText(const std::string& text) {
    this->text = text;
    return *this;
}

ButtonBuilder& ButtonBuilder::setPosition(const sf::Vector2f position) {
    this->position = position;
    return *this;
}

ButtonBuilder& ButtonBuilder::setSize(const sf::Vector2f size) {
    this->size = size;
    return *this;
}

ButtonBuilder& ButtonBuilder::setNotificationMessage(
    const std::string& message) {
    this->notificationMessage = message;
    return *this;
}

ButtonBuilder& ButtonBuilder::reset() {
    text = "";
    styleConfig = nlohmann::json();
    position = {0.f, 0.f};
    size = {0.f, 0.f};
    notificationMessage = "";
    callback = nullptr;
    return *this;
}

ButtonBuilder& ButtonBuilder::loadJson(std::string id) {
    styleConfig = JSONLoader::getInstance().getStyle(id);
    return *this;
}

std::unique_ptr<Button> ButtonBuilder::build() {
    std::unique_ptr<Button> result(
        new Button(sf::FloatRect{position, size}, mediator));

    result->setNotificationMessage(notificationMessage);
    result->style.loadJson(styleConfig, resManager);
    result->setOnClick(callback);
    std::unique_ptr<sf::Text> label =
        std::make_unique<sf::Text>(*resManager.getFont(fontName), text, 24);
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

ButtonBuilder& ButtonBuilder::setCallback(std::function<void(Button*)> call) {
    callback = call;
    return *this;
}