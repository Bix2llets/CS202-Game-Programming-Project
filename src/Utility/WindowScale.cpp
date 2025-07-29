#include "Utility/WindowScale.hpp"

#include "Base/Constants.hpp"
#include "Core/window.hpp"
sf::FloatRect WindowScale::screenScale(sf::FloatRect source) {
    source.position.x *= getXRatio();
    source.position.y *= getYRatio();
    source.size.x *= getXRatio();
    source.size.y *= getYRatio();
    return source;
}

sf::Vector2f WindowScale::screenScale(sf::Vector2f source) {
    source.x *= getXRatio();
    source.y *= getYRatio();
    return source;
}

float WindowScale::getXRatio() {
    return static_cast<float>(GameConstants::DEFAULT_WINDOW_WIDTH) /
           static_cast<float>(Window::getInstance().getRenderWindow().getSize().x);
}
float WindowScale::getYRatio() {
    return static_cast<float>(GameConstants::DEFAULT_WINDOW_HEIGHT) /
           static_cast<float>(Window::getInstance().getRenderWindow().getSize().y);
}