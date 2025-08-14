#include "Entity/Tower/Behaviors/Combat/FireMode.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/Tower/Projectile/Projectile.hpp"
#include "Scene/Level.hpp"
#include "Utility/logger.hpp"

namespace Combat {

int InstantFireMode::fire(Tower* tower, std::vector<Enemy*>& target) const {
    if (!tower) return 0;
    
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
            enemy->applyEffects(tower->getUniqueId(), *tower->getStats());
        }
    }
    
    // Update tower timer based on fire rate
    float fireRate = tower->getStat(TowerStat::FIRE_RATE, 1.0f); // Default 1.0 shots per second
    float interval = 1.0f / fireRate; // Convert fire rate to interval (seconds between shots)
    
    // Update tower timer directly (no const_cast needed anymore)
    tower->getTimer().reset(); // Reset the timer to start the cooldown

    return target.size(); // Return number of targets hit
}

void ProjectileFireMode::setProjectile(std::unique_ptr<Projectile> proj) {
    if (projectile) {
        Logger::warning("ProjectileFireMode: Overwriting existing projectile template");
    }
    projectile = std::move(proj);
}

int ProjectileFireMode::fire(Tower* tower, std::vector<Enemy*>& target) const {
    if (!tower) return 0;
    
    // Set the main target to the first enemy in the vector (index 0)
    if (!target.empty() && target[0] != nullptr) {
        tower->setMainTarget(target[0]);
    }
    
    // Check if tower is in a level (needed for projectile management)
    Level* level = tower->getLevelRef();
    if (!level) {
        Logger::warning("ProjectileFireMode: Tower is not in a Level, cannot fire projectiles");
        return 0;
    }
    
    try {
        // Check if we have a projectile template to clone from
        if (!projectile) {
            Logger::error("ProjectileFireMode: No projectile template available to clone from");
            return 0;
        }
        
        int totalFired = 0;
        for (Enemy* enemy : target) {
            if (!enemy) continue; // Skip null enemies
            
            // Clone the projectile template to create a new projectile instance
            auto firedProjectile = std::make_unique<Projectile>(*projectile);
            
            sf::Vector2f startingOffset = (enemy->getPosition() - tower->getPosition()).normalized() *
                                      tower->getStat(TowerStat::PROJECTILE_STARTING_DISTANCE, 0.0f);
            firedProjectile->setPosition(tower->getPosition() + startingOffset);

            firedProjectile->bindToTower(tower); // Bind projectile to tower (sets up damage, speed, etc.)
            firedProjectile->setTarget(enemy); // Set the target enemy for the projectile
            
            level->getEntityManager().addProjectile(std::move(firedProjectile)); // Add projectile to the level's entity manager
            
            float fireRate = tower->getStat(TowerStat::FIRE_RATE, 1.0f); // Default 1.0 shots per second
            float interval = 1.0f / fireRate; // Convert fire rate to interval (seconds between shots)
            
            tower->getTimer().reset(); // Reset the timer to start the cooldown
            
            totalFired++;
        }

        return totalFired; // Return number of projectiles fired

    } catch (const std::exception& e) {
        Logger::error("ProjectileFireMode: Failed to fire projectile - " + std::string(e.what()));
        return 0;
    }
}

int ContinuousFireMode::fire(Tower* tower, std::vector<Enemy*>& target) const {
    if (!tower) return 0;

    // Set the main target to the first enemy in the vector (index 0)
    if (!target.empty() && target[0] != nullptr) {
        tower->setMainTarget(target[0]);
    }
    
    // TODO: Implement continuous fire mode  
    // This would apply damage over time while targets remain in range
    return target.size(); // Return number of targets hit
}

}
