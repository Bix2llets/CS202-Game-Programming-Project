#include "Entity/Tower/Resource/ResourceTower.hpp"

ResourceTower::ResourceTower(const sf::Vector2f& pos, int amount, float rate)
    : Tower(pos), resourceAmount(amount), generationRate(rate), lastGenerateTime(0.0f) {
    // Initialize resource tower specific properties
}

void ResourceTower::update(float deltaTime) {
    // Implementation will be added later
    // This will generate resources over time
}
