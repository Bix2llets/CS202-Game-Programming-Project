#include "Entity/Tower/Passive/PassiveTower.hpp"

PassiveTower::PassiveTower(const sf::Vector2f& pos, float range, float power)
    : Tower(pos), effectRange(range), effectPower(power) {
    // Initialize passive tower specific properties
}

void PassiveTower::update() {
    // Implementation will be added later
    // This will apply passive effects to nearby towers or enemies
}
