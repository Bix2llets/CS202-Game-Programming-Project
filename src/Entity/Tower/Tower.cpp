#include "Entity/Tower/Tower.hpp"
#include <cmath>
#include <algorithm>

Tower::Tower(const sf::Vector2f& pos, int buildCost)
    : Entity(pos), cost(buildCost), level(1), maxLevel(3) {
    
    // Note: Base sprite will be initialized when a texture is set
}

void Tower::update() {

}


bool Tower::upgrade() {
    if (level < maxLevel) {
        level++;
        return true;
    }
    return false;
}


void Tower::loadBaseTexture(const sf::Texture& texture) {
    if (baseSprite) {
        baseSprite->setTexture(texture);
        baseSprite->setOrigin(baseSprite->getLocalBounds().size / 2.f);
        return;
    }
    baseSprite = sf::Sprite(texture);
    baseSprite->setPosition(position);
    baseSprite->setOrigin(baseSprite->getLocalBounds().size / 2.f);
}

void Tower::setPosition(const sf::Vector2f& pos) {
    Entity::setPosition(pos);
    
    if (baseSprite) {
        baseSprite->setPosition(pos);
    }
}
