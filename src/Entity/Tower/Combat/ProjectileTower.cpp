#include "Entity/Tower/Combat/ProjectileTower.hpp"

#include <iostream>

#include "Base/Constants.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Tower/Combat/TargetSelection.hpp"
ProjectileTower::ProjectileTower(const sf::Vector2f& pos, float attackRange,
                                 int attackDamage, float attackSpeed,
                                 float projSpeed, float splash)
    : CombatTower(pos, attackRange, attackDamage),
      projectileSpeed(projSpeed),
      splashRadius(splash) {
    // Initialize projectile tower specific properties
}

void ProjectileTower::executeAction(const std::vector<Enemy*>& targets) {
    if (targets.empty()) return;

    Enemy* target = targets[0];
    if (target && target->isAlive()) {
        // Launch a projectile at the target
        launchProjectile(target);
    }

    // Reset attack timer
    remainingTime = 1.f / attackSpeed;
}

void ProjectileTower::update() {
    // Call parent update first
    CombatTower::update();

    // Increment last attack time
    remainingTime -= GameConstants::TICK_INTERVAL;
    if (remainingTime < 0) remainingTime = 0;

    // Check if we can attack and have a target
    if (target && canAttack()) {
        std::vector<Enemy*> targets = {target};
        executeAction(targets);
    }
}

void ProjectileTower::launchProjectile(Enemy* target) {
    // This is a placeholder function that would create and launch a projectile
    // For now, we're just logging that a projectile was launched
    std::cout << "Projectile launched at enemy from position (" << position.x
              << ", " << position.y << ") with speed " << projectileSpeed
              << std::endl;

    // In a full implementation, this would create a Projectile object and add
    // it to the game The Projectile would then handle its own movement and
    // collision detection
}
