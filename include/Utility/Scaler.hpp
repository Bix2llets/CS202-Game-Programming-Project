#pragma once

#include <SFML/Graphics.hpp>
class Scaler {
    public:
    static sf::Vector2f screenScale(sf::Vector2f source);
    static sf::FloatRect screenScale(sf::FloatRect source);

    static sf::Sprite scaleSprite(sf::Sprite sprite, sf::Vector2f desiredSize);
    private:
    static float getScreenXRatio();
    static float getYScreenRatio();
};