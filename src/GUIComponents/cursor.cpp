#include "GUIComponents/cursor.hpp"

#include <Entity/Tower/Tower.hpp>
#include <SFML/Graphics.hpp>
#include <optional>

#include "Core/JSONLoader.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "Core/Window.hpp"
#include "Entity/Factory/TowerFactory.hpp"
#include "Utility/Logger.hpp"
#include "Utility/aligner.hpp"
std::unique_ptr<Cursor> Cursor::instance = nullptr;
Cursor::Cursor()
    : position(0.f, 0.f),
      renderImage{GameConstants::BLANK_TEXTURE},
      hoverText(*ResourceManager::getInstance().getFont("text")) {
    setValidPlacement();
}

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
    displaying = true;
    renderImage.setPosition(position);
    renderImage = Aligner::align(renderImage, HorizontalAlignment::Center,
                                 VerticalAlignment::Middle);
}

void Cursor::removeRenderImage() {
    displaying = false;
    renderImage = sf::Sprite(GameConstants::BLANK_TEXTURE);
}

void Cursor::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Draw the cursor (e.g., a circle)
    sf::CircleShape shape(cursorRadius);
    sf::CircleShape outerRing(cursorRadius + 1);
    shape.setOrigin(
        {static_cast<float>(cursorRadius), static_cast<float>(cursorRadius)});
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);

    outerRing.setOrigin({static_cast<float>(cursorRadius + 1),
                         static_cast<float>(cursorRadius + 1)});
    outerRing.setPosition(position);
    outerRing.setFillColor(sf::Color::Black);
    Window::getInstance().toggleGUIMode();
    target.draw(outerRing, states);
    target.draw(shape, states);
    target.draw(hoverText, states);
        
    // Draw tower preview if available

    if (displaying) {
        sf::RenderStates previewStates = states;
        previewStates.shader = nullptr;
        previewStates.blendMode = sf::BlendAlpha;
        // Set alpha in states, not by modifying previewTower
        // Assuming previewTower uses its own color, we can use a shader or
        // blend mode for transparency. If previewTower does not support color
        // via states, this will only affect blending.
        Window::getInstance().toggleUserMode();
        target.draw(rangePreview);
        target.draw(renderImage, previewStates);
        // sf::RectangleShape border(renderImage.getLocalBounds().size);
        // Aligner::align(border, HorizontalAlignment::Center,
        //                VerticalAlignment::Middle);
        // border.setPosition(renderImage.getPosition());
        // border.setFillColor(sf::Color::Transparent);
        // border.setOutlineColor(sf::Color::Red);
        // border.setOutlineThickness(1.f);
        // target.draw(border);

        // float attackRadius = previewTower->getStat("range", 0);
        // sf::CircleShape attackPreview(radius);
        // attackPreview.setFillColor({255, 255, 255, 127});
    }
}

bool Cursor::onMouseEvent(Mouse mouse, UserEvent event,
                          const sf::Vector2f& worldPosition,
                          const sf::Vector2f& windowPosition) {
    if (event == UserEvent::Move) {
        position = windowPosition;
        renderImage.setPosition(worldPosition);
        rangePreview.setPosition(worldPosition);

        sf::Vector2f offset(10.f, -10.f);
        if (windowPosition.x + offset.x + hoverText.getGlobalBounds().size.x >
            GameConstants::MENU_X) {
            offset.x = -10.f - hoverText.getGlobalBounds().size.x;
        }
        if (windowPosition.y + offset.y + hoverText.getGlobalBounds().size.y < 0) {
            offset.y = 10.f + hoverText.getGlobalBounds().size.y;
        }
        hoverText.setPosition(windowPosition + offset);
        // Logger::debug("processing moues movent in cursor");

        return true;
    }
    return false;
}

void Cursor::setCarryingTower(std::string id) {
    carryingTowerId = id;
    rangePreview.setRadius(
        JSONLoader::getInstance().getTower(id)["stats"]["range"]);
    rangePreview.setOrigin(
        {rangePreview.getRadius(), rangePreview.getRadius()});
    Window::getInstance().toggleUserMode();
    sf::Vector2f worldPosition =
        Window::getInstance().getRenderWindow().mapPixelToCoords(
            sf::Mouse::getPosition(Window::getInstance().getRenderWindow()));
    rangePreview.setPosition(worldPosition);

    if (previewTower.resize({64, 64})) {
        // Successfully resized the preview tower
        Logger::debug("Preview tower resized successfully.");
    }
}
void Cursor::clearCarryingTower() {
    carryingTowerId = "";
    rangePreview.setRadius(0);
}

bool Cursor::onScrollEvent(float delta, const sf::Vector2f& worldPosition,
                           const sf::Vector2f& windowPosition) {
    return false;
}

void Cursor::setValidPlacement() {
    sf::Color color = sf::Color::White;
    color.a = 127;
    sf::Color previewColor = sf::Color::Black;
    previewColor.a = 127;
    renderImage.setColor(previewColor);

    rangePreview.setFillColor(previewColor);
}

void Cursor::setInvalidPlacement() {
    sf::Color color = sf::Color::Red;
    color.a = 127;
    renderImage.setColor(color);
    rangePreview.setFillColor(color);
}

void Cursor::setHoverText(const std::string& text) {
    // Implement hover text display logic here
    hoverText.setString(text);
    hoverText.setCharacterSize(14);            // Set appropriate character size
    hoverText.setFillColor(sf::Color::White);  // Set text color
    hoverText.setOutlineColor(sf::Color::Black);  // Set outline color
    hoverText.setOutlineThickness(1.f);           // Set outline thickness
    Aligner::align(hoverText, HorizontalAlignment::Left,
                   VerticalAlignment::Bottom);
}