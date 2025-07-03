#include "Entity/Tower/Tower.hpp"
#include <cmath>
#include <algorithm>

Tower::Tower(const sf::Vector2f& pos, int buildCost)
    : Entity(pos), cost(buildCost), level(1), maxLevel(3) {
    
    // Note: Base sprite will be initialized when a texture is set
}

void Tower::update(float deltaTime) {
    // Base tower update logic
    // This will be overridden by specific tower types
}

void Tower::render(sf::RenderTarget& target) {
    // Render base sprite first (so it appears behind the upper layer)
    if (baseSprite.has_value()) {
        target.draw(baseSprite.value());
    }
    
    // Then render the upper rotating part (inherited from Entity)
    Entity::render(target);
    
    // Additional rendering for tower-specific elements could go here
    // For example, range circles, targeting lines, etc.
}

bool Tower::upgrade() {
    if (level < maxLevel) {
        level++;
        return true;
    }
    return false;
}

void Tower::setBaseSprite(const sf::Sprite& newBaseSprite) {
    baseSprite = newBaseSprite;
    if (baseSprite.has_value()) {
        baseSprite->setPosition(position);
        // Base sprite doesn't rotate, so we don't set rotation
    }
}

void Tower::setBaseSpriteTexture(const sf::Texture& texture) {
    baseSprite = sf::Sprite(texture);
    if (baseSprite.has_value()) {
        baseSprite->setPosition(position);
        // Base sprite doesn't rotate, so we don't set rotation
    }
}

void Tower::setPosition(const sf::Vector2f& pos) {
    // Call the parent's implementation to update position and the upper layer sprite
    Entity::setPosition(pos);
    
    // Also update the base sprite position
    if (baseSprite.has_value()) {
        baseSprite->setPosition(pos);
    }
}
