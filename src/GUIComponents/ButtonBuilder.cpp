#include "GUIComponents/ButtonBuilder.hpp"

ButtonBuilder::ButtonBuilder(Mediator& mediator,
                             ResourceManager& resManager,
                             JSONLoader& loader)
    : mediator{mediator}, resManager{resManager}, loader{loader} {}

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
    styleConfig = loader.getStyle(id);
    return *this;
}

std::unique_ptr<Button> ButtonBuilder::build() {
    std::unique_ptr<Button> result =
        std::make_unique<Button>(text, sf::FloatRect{position, size}, mediator);

    result->setNotificationMessage(notificationMessage);
    result->style.loadJson(styleConfig, resManager);
    result->setOnClick(callback);

    return std::move(result);
}

ButtonBuilder& ButtonBuilder::setCallback(std::function<void(Button*)> call) {
    callback = call;
    return *this;
}