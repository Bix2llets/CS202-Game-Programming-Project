#pragma once

#include <SFML/Graphics.hpp>
class WindowScale {
    public:
    static sf::Vector2f screenScale(sf::Vector2f source);
    static sf::FloatRect screenScale(sf::FloatRect source);
    private:
    static float getXRatio();
    static float getYRatio();
};