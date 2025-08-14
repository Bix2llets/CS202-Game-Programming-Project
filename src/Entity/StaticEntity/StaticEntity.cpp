/**
 * @file StaticEntity.cpp
 * @brief Implementation of the StaticEntity class.
 *
 * This file contains the implementation of methods for managing static entities
 * that have texture, dimensions, and removal costs.
 */

#include "Entity/StaticEntity/StaticEntity.hpp"
#include "Scene/Scene.hpp"
#include "Utility/logger.hpp"
#include "Utility/Scaler.hpp"
#include "Scene/Level.hpp"

StaticEntity::StaticEntity(Scene& scene) : Entity(scene) {
    levelRef = dynamic_cast<Level*>(&scene);
}

void StaticEntity::update() {
    // Static entities typically don't need to update their state
    // This method can be overridden in derived classes if needed
}

void StaticEntity::setPosition(const sf::Vector2f& pos) {
    position = pos;
    sprite.setPosition(pos);
}

void StaticEntity::setRotation(const sf::Angle& rot) {
    rotation = rot;
    sprite.setRotation(rot);
}

void  StaticEntity::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    target.draw(sprite, state);
}

void StaticEntity::setTextureWidth(float width) {
    textureWidth = width;
}

void StaticEntity::setTextureHeight(float height) {
    textureHeight = height;
}

void StaticEntity::setRemoveCost(const Currency& cost) {
    removeCost = cost;
}

bool StaticEntity::canBeRemoved(const Currency& availableCurrency) const {
    return availableCurrency.canAfford(removeCost);
}

void StaticEntity::loadSpriteTexture(const sf::Texture& texture) {
    sprite = sf::Sprite(texture);
    sf::Vector2u originalSize = texture.getSize();
    sf::Vector2f origin = sf::Vector2f(originalSize.x / 2.f, originalSize.y / 2.f);
    sprite.setOrigin(origin);
    sprite = Scaler::scaleSprite(sprite, {textureWidth, textureHeight});

    sprite.setPosition(position);
    sprite.setRotation(rotation);
}

sf::FloatRect StaticEntity::getBounds() const {
    return sprite.getGlobalBounds();
}
