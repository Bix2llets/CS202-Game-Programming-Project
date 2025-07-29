#include "GUIComponents/cursor.hpp"

#include <Entity/Tower/Tower.hpp>
#include <SFML/Graphics.hpp>
#include <optional>

#include "Core/UserEvent.hpp"
#include "Entity/Factory/TowerFactory.hpp"
#include "Utility/Logger.hpp"

#include "Core/Window.hpp"
std::unique_ptr<Cursor> Cursor::instance = nullptr;
Cursor::Cursor() : position(0.f, 0.f), renderImage{GameConstants::BLANK_TEXTURE} {}

Cursor& Cursor::getInstance() {
    static Cursor instance;
    return instance;
}

void Cursor::setPosition(const sf::Vector2f& pos) {
    position = pos;

    renderImage.setPosition(pos);
}

const sf::Vector2f& Cursor::getPosition() const { return position; }

void Cursor::setRenderImage(sf::Sprite sprite) {
    renderImage = sprite;
    isDisplaying = true;
    // ! Assume there is a way to construct tower from its id
}

void Cursor::removeRenderImage() {
    isDisplaying = false;
    renderImage = sf::Sprite(GameConstants::BLANK_TEXTURE);
}

void Cursor::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Draw the cursor (e.g., a circle)
    sf::CircleShape shape(cursorRadius);
    sf::CircleShape outerRing(cursorRadius + 1);
    shape.setOrigin({static_cast<float>(cursorRadius), static_cast<float>(cursorRadius)});
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);

    outerRing.setOrigin(
        {static_cast<float>(cursorRadius + 1), static_cast<float>(cursorRadius + 1)});
    outerRing.setPosition(position);
    outerRing.setFillColor(sf::Color::Black);
    Window::getInstance().toggleGUIMode();
    target.draw(outerRing, states);
    target.draw(shape, states);

    // Draw tower preview if available

    if (isDisplaying) {
        sf::RenderStates previewStates = states;
        previewStates.shader = nullptr;
        previewStates.blendMode = sf::BlendAlpha;
        // Set alpha in states, not by modifying previewTower
        sf::Color color = sf::Color::White;
        color.a = 128;  // semi-transparent
        // Assuming previewTower uses its own color, we can use a shader or
        // blend mode for transparency. If previewTower does not support color
        // via states, this will only affect blending.
        target.draw(renderImage, previewStates);

        // float attackRadius = previewTower->getStat("range", 0);
        // sf::CircleShape attackPreview(radius);
        // attackPreview.setFillColor({255, 255, 255, 127});
    }
}

void Cursor::onMouseEvent(Mouse mouse, UserEvent event,
                          const sf::Vector2f& worldPosition,
                          const sf::Vector2f& windowPosition) {
    if (event == UserEvent::Move) {
        position = windowPosition;
        renderImage.setPosition(position);
        // Logger::debug("processing moues movent in cursor");

        return;
    }
}

void Cursor::onScrollEvent(float delta, const sf::Vector2f& worldPosition,
                           const sf::Vector2f& windowPosition) {}