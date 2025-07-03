#include "Entity/Entity.hpp"

int Entity::nextEntityId = 1;

Entity::Entity(const sf::Vector2f& pos)
    : position(pos), rotation(sf::degrees(0.0f)), alive(true), entityId(nextEntityId++) {
    // Sprite will be initialized when a texture is set
}

void Entity::render(sf::RenderTarget& target) {
    if (alive && sprite.has_value()) {
        target.draw(sprite.value());
    }
}

void Entity::setPosition(const sf::Vector2f& pos) {
    position = pos;
    if (sprite.has_value()) {
        sprite->setPosition(position);
    }
}

void Entity::setRotation(const sf::Angle& rot) {
    rotation = rot;
    if (sprite.has_value()) {
        sprite->setRotation(rotation);
    }
}

void Entity::setSprite(const sf::Sprite& newSprite) {
    sprite = newSprite;
    if (sprite.has_value()) {
        sprite->setPosition(position);
        sprite->setRotation(rotation);
    }
}

void Entity::setSpriteTexture(const sf::Texture& texture) {
    sprite = sf::Sprite(texture);
    if (sprite.has_value()) {
        sprite->setPosition(position);
        sprite->setRotation(rotation);
    }
}
