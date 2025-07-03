#include "Entity/Tower/Resource/ResourceTower.hpp"

ResourceTower::ResourceTower(const sf::Vector2f& pos, int amount, float rate)
    : Tower(pos), totalResource(amount), generationRate(rate), remainingTime(0.0f) {
    // Initialize resource tower specific properties
}

void ResourceTower::update() {
    // Implementation will be added later
    // This will generate resources over time
}
