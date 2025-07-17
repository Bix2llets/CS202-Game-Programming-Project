#include "GUIComponents/cursor.hpp"

#include <Entity/Tower/Tower.hpp>
#include <SFML/Graphics.hpp>
#include <optional>

#include "Core/UserEvent.hpp"
#include "Entity/Factory/TowerFactory.hpp"
std::unique_ptr<Cursor> Cursor::instance = nullptr;
Cursor::Cursor() : position(0.f, 0.f) {}

Cursor* Cursor::getInstance() {
    if (instance == nullptr) instance = std::unique_ptr<Cursor>(new Cursor);
    return instance.get();
}

void Cursor::setPosition(const sf::Vector2f& pos) {
    position = pos;
    previewTower->setPosition(pos);
}

const sf::Vector2f& Cursor::getPosition() const { return position; }

void Cursor::setTower(const std::string& id) {
    // ! Assume there is a way to construct tower from its id
}

void Cursor::removeTower() {
    previewTower.reset();
    previewTower = nullptr;
}

void Cursor::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Draw the cursor (e.g., a circle)
    sf::CircleShape shape(radius);
    sf::CircleShape outerRing(radius + 1);
    shape.setOrigin({static_cast<float>(radius), static_cast<float>(radius)});
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);

    outerRing.setOrigin({static_cast<float>(radius + 1), static_cast<float>(radius + 1)});
    outerRing.setPosition(position);
    outerRing.setFillColor(sf::Color::Black);
    target.draw(outerRing, states);
    target.draw(shape, states);

    // Draw tower preview if available
    if (previewTower) {
        sf::RenderStates previewStates = states;
        previewStates.shader = nullptr;
        previewStates.blendMode = sf::BlendAlpha;
        // Set alpha in states, not by modifying previewTower
        sf::Color color = sf::Color::White;
        color.a = 128;  // semi-transparent
        // Assuming previewTower uses its own color, we can use a shader or
        // blend mode for transparency. If previewTower does not support color
        // via states, this will only affect blending.
        target.draw(*previewTower, previewStates);

        float attackRadius = previewTower->getStat("range", 0);
        sf::CircleShape attackPreview(radius);
        attackPreview.setFillColor({255, 255, 255, 127});
    }
}

void Cursor::onMouseEvent(Mouse mouse, UserEvent event,
                        const sf::Vector2f& worldPosition,
                        const sf::Vector2f& windowPosition) {
    if (event == UserEvent::Move) {
        position = windowPosition;
        return;
    }
}