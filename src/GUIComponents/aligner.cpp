#include "Utility/aligner.hpp"

sf::Text Aligner::align(sf::Text text, HorizontalAlignment hAlign,
                        VerticalAlignment vAlign) {
    sf::FloatRect bounds = text.getLocalBounds();
    sf::Vector2f position = text.getPosition();

    if (hAlign == HorizontalAlignment::Center) {
        position.x -= bounds.size.x / 2;
    } else if (hAlign == HorizontalAlignment::Right) {
        position.x -= bounds.size.x;
    }

    if (vAlign == VerticalAlignment::Middle) {
        position.y -= bounds.size.y / 2;
    } else if (vAlign == VerticalAlignment::Bottom) {
        position.y -= bounds.size.y;
    }

    text.setOrigin(position);
    return text;
}

sf::Sprite Aligner::align(sf::Sprite sprite, HorizontalAlignment hAlign,
                        VerticalAlignment vAlign) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sf::Vector2f position = sprite.getPosition();

    if (hAlign == HorizontalAlignment::Center) {
        position.x -= bounds.size.x / 2;
    } else if (hAlign == HorizontalAlignment::Right) {
        position.x -= bounds.size.x;
    }

    if (vAlign == VerticalAlignment::Middle) {
        position.y -= bounds.size.y / 2;
    } else if (vAlign == VerticalAlignment::Bottom) {
        position.y -= bounds.size.y;
    }

    sprite.setOrigin(position);
    return sprite;
}