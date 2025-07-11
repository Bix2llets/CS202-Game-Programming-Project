#include "Entity/Tower/Tower.hpp"
#include <cmath>
#include <algorithm>
#include <iostream> // Include for debug output

void Tower::addBehavior(std::unique_ptr<TowerBehavior> behavior) {
    for (auto& ex : behaviors) {
        if (ex->getType() == behavior->getType()) {
            // If a behavior of the same type already exists, remove it
            behaviors.erase(
                std::remove(behaviors.begin(), behaviors.end(), ex),
                behaviors.end());
            break;  // Only remove one instance
        }  
    }
    behaviors.push_back(std::move(behavior));
}

const TowerStat* Tower::getStats() const {
    return stats.get();
}

TowerStat* Tower::getStats() {
    return stats.get();
}

float Tower::getStat(const std::string& statName, float defaultValue) const {
    return stats->getStatWithBonus(statName, defaultValue);
}

void Tower::setTimerInterval(float interval) {
    timer.setInterval(interval);
}

void Tower::setStats(std::unique_ptr<TowerStat> newStats) {
    stats = std::move(newStats);
}

// Implementation of dual sprite system methods
void Tower::setPosition(const sf::Vector2f& pos) {
    position = pos;
    
    // Update base sprite position if it exists
    base.setPosition(pos);
    
    // Update turret sprite position if it exists
    sprite.setPosition(pos);
}

void Tower::setRotation(const sf::Angle& rot) {
    setTurretRotation(rot);
}

void Tower::setBaseRotation(const sf::Angle& rot) {
    baseRotation = rot;
    
    // Update base sprite rotation if it exists
    base.setRotation(rot);
}

void Tower::setTurretRotation(const sf::Angle& rot) {
    rotation = rot;  // Update Entity's rotation
    
    // Update turret sprite rotation if it exists
    sprite.setRotation(rot);
}

void Tower::loadBaseSpriteTexture(const sf::Texture& texture) {
    base = sf::Sprite(texture);
    
    // Get original texture size
    sf::Vector2u originalSize = texture.getSize();
    
    // Set origin to center of ORIGINAL texture size (before scaling)
    sf::Vector2f origin(originalSize.x / 2.0f, originalSize.y / 2.0f);
    base.setOrigin(origin);
    
    // Scale sprite to desired dimensions
    float scaleX = textureWidth / static_cast<float>(originalSize.x);
    float scaleY = textureHeight / static_cast<float>(originalSize.y);
    base.setScale(sf::Vector2f(scaleX, scaleY));
    
    // Ensure position and rotation are set correctly after scaling
    base.setPosition(position);
    base.setRotation(baseRotation);  // Use the stored base rotation
}

void Tower::loadTurretSpriteTexture(const sf::Texture& texture) {
    loadSpriteTexture(texture);  // Use Entity's method
    // Get original texture size
    sf::Vector2u originalSize = texture.getSize();
    
    // Set origin to center of ORIGINAL texture size (before scaling)
    sprite.setOrigin(sf::Vector2f(originalSize.x / 2.0f, originalSize.y / 2.0f));
    
    // Scale sprite to desired dimensions
    float scaleX = textureWidth / static_cast<float>(originalSize.x);
    float scaleY = textureHeight / static_cast<float>(originalSize.y);
    sprite.setScale(sf::Vector2f(scaleX, scaleY));
    
    // Ensure position and rotation are set correctly after scaling
    sprite.setPosition(position);
    sprite.setRotation(rotation);  // Use Entity's rotation for turret
}

void Tower::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    // Draw base sprite first (if it exists)
    target.draw(base, state);
    
    // Draw turret sprite on top (if it exists)
    target.draw(sprite, state);
}

void Tower::update() {
    // Reduce cooldown based on elapsed time
    // Note: In a real implementation, you would pass delta time or use a clock
    // For now, we'll use a simple approach
    
    // Update behaviors (if they have specific update logic in derived classes)
    // For now, behaviors don't have update methods, so we skip this
    
    // The timer cooldown would typically be updated with delta time in the main game loop
    // This is just a placeholder implementation to satisfy the pure virtual requirement
}