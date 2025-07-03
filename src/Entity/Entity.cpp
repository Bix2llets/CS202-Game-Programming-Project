#include "Entity/Entity.hpp"

int Entity::nextEntityId = 1;

Entity::Entity(const sf::Vector2f& position, const sf::Angle& rotation)
    : position(position), rotation(rotation), entityId(nextEntityId++) {}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    if (sprite.has_value()) target.draw(*sprite);
}
void Entity::setPosition(const sf::Vector2f& pos) {
    position = pos;
    if (sprite.has_value()) sprite->setPosition(position);
}

void Entity::setRotation(const sf::Angle& rot) {
    rotation = rot;
    if (sprite.has_value()) sprite->setRotation(rotation);
}

void Entity::loadTexture(const sf::Texture& texture) {
    if (sprite.has_value())
        sprite->setTexture(texture);
    else {
        sprite = sf::Sprite(texture);
        sprite->setRotation(rotation);
        sprite->setPosition(position);
    }
}
