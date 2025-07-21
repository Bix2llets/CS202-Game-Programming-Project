#include "Entity/Entity.hpp"

void Entity::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    target.draw(sprite);
}
void Entity::setPosition(const sf::Vector2f& pos) {
    position = pos;
    sprite.setPosition(position);
}

void Entity::setRotation(const sf::Angle& rot) {
    rotation = rot;
    sprite.setRotation(rotation);
}

void Entity::loadSpriteTexture(const sf::Texture& texture) {
    sprite = sf::Sprite(texture);
    sprite.setRotation(rotation);
    sprite.setPosition(position);
}

bool Entity::contains(sf::Vector2f position) {
    return sprite.getGlobalBounds().contains(position);
}