#include "Utility/aligner.hpp"

sf::Text Aligner::align(sf::Text text, HorizontalAlignment hAlign,
                        VerticalAlignment vAlign) {
    sf::FloatRect bounds = text.getLocalBounds();
    sf::Vector2f origin = {0.f, 0.f};

    if (hAlign == HorizontalAlignment::Center) {
        origin.x -= bounds.size.x / 2;
    } else if (hAlign == HorizontalAlignment::Right) {
        origin.x -= bounds.size.x;
    }

    if (vAlign == VerticalAlignment::Middle) {
        origin.y -= bounds.size.y / 2;
    } else if (vAlign == VerticalAlignment::Bottom) {
        origin.y -= bounds.size.y;
    }

    text.setOrigin(bounds.position + origin);
    return text;
}

sf::Sprite Aligner::align(sf::Sprite sprite, HorizontalAlignment hAlign,
                        VerticalAlignment vAlign) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sf::Vector2f origin = sf::Vector2f(0, 0);

    if (hAlign == HorizontalAlignment::Center) {
        origin.x += bounds.size.x / 2;
    } else if (hAlign == HorizontalAlignment::Right) {
        origin.x += bounds.size.x;
    }

    if (vAlign == VerticalAlignment::Middle) {
        origin.y += bounds.size.y / 2;
    } else if (vAlign == VerticalAlignment::Bottom) {
        origin.y += bounds.size.y;
    }

    sprite.setOrigin(bounds.position + origin);
    return sprite;
}

sf::RectangleShape Aligner::align(sf::RectangleShape shape,
                                 HorizontalAlignment hAlign,
                                 VerticalAlignment vAlign) {
    sf::FloatRect bounds = shape.getLocalBounds();
    sf::Vector2f origin = {0, 0};

    if (hAlign == HorizontalAlignment::Center) {
        origin.x += bounds.size.x / 2;
    } else if (hAlign == HorizontalAlignment::Right) {
        origin.x += bounds.size.x;
    }

    if (vAlign == VerticalAlignment::Middle) {
        origin.y += bounds.size.y / 2;
    } else if (vAlign == VerticalAlignment::Bottom) {
        origin.y += bounds.size.y;
    }

    shape.setOrigin(bounds.position + origin);
    return shape;
}

sf::CircleShape Aligner::align(sf::CircleShape shape,
                                 HorizontalAlignment hAlign,
                                 VerticalAlignment vAlign) {
    sf::FloatRect bounds = shape.getLocalBounds();
    sf::Vector2f origin = {0, 0};

    if (hAlign == HorizontalAlignment::Center) {
        origin.x += bounds.size.x / 2;
    } else if (hAlign == HorizontalAlignment::Right) {
        origin.x += bounds.size.x;
    }

    if (vAlign == VerticalAlignment::Middle) {
        origin.y += bounds.size.y / 2;
    } else if (vAlign == VerticalAlignment::Bottom) {
        origin.y += bounds.size.y;
    }

    shape.setOrigin(bounds.position + origin);
    return shape;
}