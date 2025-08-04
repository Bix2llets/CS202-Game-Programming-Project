#pragma once

#include <SFML/Graphics.hpp>

enum class HorizontalAlignment { Left, Center, Right };

enum class VerticalAlignment { Top, Middle, Bottom };
class Aligner {
    public:
    static sf::Text align(
        sf::Text text, HorizontalAlignment hAlign = HorizontalAlignment::Center,
        VerticalAlignment vAlign = VerticalAlignment::Middle);

    static sf::Sprite align(
        sf::Sprite sprite,
        HorizontalAlignment hAlign = HorizontalAlignment::Center,
        VerticalAlignment vAlign = VerticalAlignment::Middle);

    static sf::RectangleShape align(
        sf::RectangleShape shape,
        HorizontalAlignment hAlign = HorizontalAlignment::Center,
        VerticalAlignment vAlign = VerticalAlignment::Middle);

    static sf::CircleShape align(
        sf::CircleShape shape,
        HorizontalAlignment hAlign = HorizontalAlignment::Center,
        VerticalAlignment vAlign = VerticalAlignment::Middle);
};