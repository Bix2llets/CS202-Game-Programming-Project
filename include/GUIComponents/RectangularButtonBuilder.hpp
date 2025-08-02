#pragma once

#include <memory>
#include <string>

#include "Core/JSONLoader.hpp"
#include "GUIComponents/RectangularButton.hpp"
#include "GUIComponents/Style.hpp"
// RectangularButtonBuilder: Implements the builder pattern for Button GUI components.
class RectangularButtonBuilder {
   private:
    nlohmann::json styleConfig;

    std::string text;
    sf::Vector2f position;
    sf::Vector2f size;
    std::string notificationMessage;

    Mediator& mediator;

    std::function<void(RectangularButton*)> callback;

    std::string fontName;
    const sf::Texture* tex;

   public:
    RectangularButtonBuilder(Mediator& mediator);

    RectangularButtonBuilder& setText(const std::string& text);

    RectangularButtonBuilder& setPosition(sf::Vector2f position);

    RectangularButtonBuilder& setSize(sf::Vector2f size);

    RectangularButtonBuilder& reset();

    RectangularButtonBuilder& loadJson(std::string id);

    RectangularButtonBuilder& setCallback(std::function<void(RectangularButton*)> call);

    RectangularButtonBuilder& setBackground(const sf::Texture* tex);

    std::unique_ptr<RectangularButton> build() ;
};