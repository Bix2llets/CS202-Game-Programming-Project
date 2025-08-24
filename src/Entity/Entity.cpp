#include "Entity/Entity.hpp"

#include "Utility/logger.hpp"
#include "Utility/Scaler.hpp"
#include "Scene/Level.hpp"

Entity::Entity(Scene &scene, const sf::Texture& texture) : scene(scene), sprite(texture) {
    uniqueId = static_cast<int64_t>(reinterpret_cast<std::uintptr_t>(this));
    levelRef = dynamic_cast<Level*>(&scene);
}

Entity::Entity(Scene &scene) : scene(scene), sprite(GameConstants::BLANK_TEXTURE) {
    uniqueId = static_cast<int64_t>(reinterpret_cast<std::uintptr_t>(this));
    levelRef = dynamic_cast<Level*>(&scene);
}

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
    // Logger::debug(std::format("Cursor position: ({}, {})",
    //                           position.x, position.y));
    auto bounds = Scaler::screenScale(sprite.getGlobalBounds());
    // Logger::debug(std::format(
    //     "Sprite global bounds: left={}, top={}, width={}, height={}",
    //     bounds.position.x, bounds.position.y, bounds.size.x, bounds.size.y));
    return bounds.contains(position);
}