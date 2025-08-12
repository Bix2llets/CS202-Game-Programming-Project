#include "Utility/Scaler.hpp"

#include "Base/Constants.hpp"
#include "Core/window.hpp"
sf::FloatRect Scaler::screenScale(sf::FloatRect source) {
    source.position.x *= getScreenXRatio();
    source.position.y *= getYScreenRatio();
    source.size.x *= getScreenXRatio();
    source.size.y *= getYScreenRatio();
    return source;
}

sf::Vector2f Scaler::screenScale(sf::Vector2f source) {
    source.x *= getScreenXRatio();
    source.y *= getYScreenRatio();
    return source;
}

float Scaler::getScreenXRatio() {
    return static_cast<float>(GameConstants::DEFAULT_WINDOW_WIDTH) /
           static_cast<float>(
               Window::getInstance().getRenderWindow().getSize().x);
}
float Scaler::getYScreenRatio() {
    return static_cast<float>(GameConstants::DEFAULT_WINDOW_HEIGHT) /
           static_cast<float>(
               Window::getInstance().getRenderWindow().getSize().y);
}

sf::Sprite& Scaler::scaleSprite(sf::Sprite& sprite, sf::Vector2f desiredSize) {
    auto bounds = sprite.getLocalBounds();
    float xScale = desiredSize.x / bounds.size.x;
    float yScale = desiredSize.y / bounds.size.y;
    sprite.setScale({xScale, yScale});
    return sprite;
}