#pragma once

#include <memory>
#include <string>

#include "Core/JSONLoader.hpp"
#include "GUIComponents/Button.hpp"
#include "GUIComponents/Style.hpp"
// ButtonBuilder: Implements the builder pattern for Button GUI components.
class ButtonBuilder {
   private:
    nlohmann::json styleConfig;

    std::string text;
    sf::Vector2f position;
    sf::Vector2f size;
    std::string notificationMessage;

    Mediator& mediator;

    std::function<void(Button*)> callback;

    const std::string fontName = "LeagueSpartan";

   public:
    ButtonBuilder(Mediator& mediator);

    ButtonBuilder& setText(const std::string& text);

    ButtonBuilder& setPosition(sf::Vector2f position);

    ButtonBuilder& setSize(sf::Vector2f size);

    ButtonBuilder& setNotificationMessage(const std::string &message);

    ButtonBuilder& reset();

    ButtonBuilder& loadJson(std::string id);

    ButtonBuilder& setCallback(std::function<void(Button*)> call);

    std::unique_ptr<Button> build() ;
};