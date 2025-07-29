#include "Entity/Tower/Behaviors/Combat/FireMode.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/Enemy/Enemy.hpp"

namespace Combat {

void InstantFireMode::fire(Tower* tower, std::vector<Enemy*>& target) const {
    if (!tower) return;
    
    // Set the main target to the first enemy in the vector (index 0)
    if (!target.empty() && target[0] != nullptr) {
        tower->setMainTarget(target[0]);
    }
    
    // Get damage from tower stats
    float damageValue = tower->getStat(TowerStat::DAMAGE, 0.0f);
    int damage = static_cast<int>(damageValue);
    
    // Damage all enemies in the target vector
    for (Enemy* enemy : target) {
        if (enemy) {
            enemy->onHit(damage);
        }
    }
    
    // Update tower timer based on fire rate
    float fireRate = tower->getStat(TowerStat::FIRE_RATE, 1.0f); // Default 1.0 shots per second
    float interval = 1.0f / fireRate; // Convert fire rate to interval (seconds between shots)
    
    // Update tower timer directly (no const_cast needed anymore)
    tower->setTimerInterval(interval);
    tower->getTimer().reset(); // Reset the timer to start the cooldown
}

void ProjectileFireMode::fire(Tower* tower, std::vector<Enemy*>& target) const {
    if (!tower) return;
    
    // Set the main target to the first enemy in the vector (index 0)
    if (!target.empty() && target[0] != nullptr) {
        tower->setMainTarget(target[0]);
    }
    
    // TODO: Implement projectile fire mode
    // This would create projectile entities that travel to targets
}

void ContinuousFireMode::fire(Tower* tower, std::vector<Enemy*>& target) const {
    if (!tower) return;
    
    // Set the main target to the first enemy in the vector (index 0)
    if (!target.empty() && target[0] != nullptr) {
        tower->setMainTarget(target[0]);
    }
    
    // TODO: Implement continuous fire mode  
    // This would apply damage over time while targets remain in range
}

}
