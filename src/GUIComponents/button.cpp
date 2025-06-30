#include "GUIComponents/button.hpp"

#include "Core/ResourceManager.hpp"

#include <SFML/Graphics.hpp>
#include "Base/Constants.hpp"
Button::Button(const std::string& textLabel, sf::FloatRect geometricInfo,
               Mediator& mediator)
    : geometricInfo(geometricInfo), mediator(mediator),
    onClickMessage("ButtonClicked") {
        label = std::make_unique<sf::Text>(*mediator.getFont("LeagueSpartan"), textLabel, 24);
        label->setOrigin(label->getLocalBounds().position + label->getLocalBounds().size / 2.f);
        label->setPosition(geometricInfo.position + geometricInfo.size / 2.f);
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
    rect.setFillColor(sf::Color::Blue);

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

const std::string& Button::getLabel() const { return label->getString(); }

sf::Vector2f Button::getPosition() const { return geometricInfo.position; }

void Button::onMouseEvent(Mouse button, UserEvent event,
                          const sf::Vector2f& worldPosition,
                          const sf::Vector2f& windowPosition) {
    if (geometricInfo.contains(static_cast<sf::Vector2f>(windowPosition)))
        click();
}

void Button::setNotificationMessage(const std::string &str) {
    onClickMessage = str;
}