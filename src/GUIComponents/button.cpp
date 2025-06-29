#include "GUIComponents/button.hpp"

#include "Core/ResourceManager.hpp"

#include <SFML/Graphics.hpp>
Button::Button(const std::string& label, sf::FloatRect geometricInfo,
               Mediator& mediator)
    : label(label), geometricInfo(geometricInfo), mediator(mediator) {}

void Button::setOnClick(const std::function<void(Button*)>& callback) {
    onClick = callback;
}

void Button::click() {
    mediator.notify("ButtonClicked", this);
    if (onClick) onClick(this);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::RectangleShape rect({100, 40});
    rect.setPosition(geometricInfo.position);
    rect.setFillColor(sf::Color::Blue);
    // sf::View prevView = target.getView();
    // target.setView(target.getDefaultView());
    target.draw(rect, states);
    // target.setView(prevView);
    

    // Label drawing omitted for brevity
}

const std::string& Button::getLabel() const { return label; }

sf::Vector2f Button::getPosition() const { return geometricInfo.position; }

void Button::onMouseEvent(Mouse button, UserEvent event,
                          const sf::Vector2f& worldPosition,
                          const sf::Vector2i& windowPosition) {
    if (geometricInfo.contains(static_cast<sf::Vector2f>(windowPosition)))
        click();
}