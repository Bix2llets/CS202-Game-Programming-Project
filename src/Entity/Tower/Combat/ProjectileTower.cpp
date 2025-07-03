#include "Entity/Tower/Combat/ProjectileTower.hpp"
#include "Entity/Tower/Combat/TargetSelection.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include <iostream>

ProjectileTower::ProjectileTower(const sf::Vector2f& pos, float attackRange, int attackDamage, 
                               float attackSpeed, float projSpeed, float splash)
    : CombatTower(pos, attackRange, attackDamage), 
      projectileSpeed(projSpeed), splashRadius(splash), 
      attackSpeed(attackSpeed), lastAttackTime(0.0f) {
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
    lastAttackTime = 0.0f;
}

void ProjectileTower::update(float deltaTime) {
    // Call parent update first
    CombatTower::update(deltaTime);
    
    // Increment last attack time
    lastAttackTime += deltaTime;
    
    // Check if we can attack and have a target
    if (target && canAttack(lastAttackTime)) {
        std::vector<Enemy*> targets = {target};
        executeAction(targets);
    }
}

bool ProjectileTower::canAttack(float currentTime) const {
    // Check if enough time has passed since last attack based on attack speed
    return (currentTime >= (1.0f / attackSpeed));
}

void ProjectileTower::launchProjectile(Enemy* target) {
    // This is a placeholder function that would create and launch a projectile
    // For now, we're just logging that a projectile was launched
    std::cout << "Projectile launched at enemy from position (" 
              << position.x << ", " << position.y << ") with speed " 
              << projectileSpeed << std::endl;
              
    // In a full implementation, this would create a Projectile object and add it to the game
    // The Projectile would then handle its own movement and collision detection
}
