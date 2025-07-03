#include "Entity/Tower/Combat/InstantTower.hpp"
#include "Entity/Tower/Combat/TargetSelection.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include <cmath>

InstantTower::InstantTower(const sf::Vector2f& pos, float attackRange, int attackDamage, 
                         float attackSpeed, float aoe)
    : CombatTower(pos, attackRange, attackDamage), 
      areaOfEffect(aoe), attackSpeed(attackSpeed), lastAttackTime(0.0f) {
    // Initialize instant tower specific properties
}

void InstantTower::executeAction(const std::vector<Enemy*>& targets) {
    if (targets.empty()) return;
    
    if (areaOfEffect <= 0) {
        // Single target attack
        Enemy* target = targets[0];
        if (target && target->isAlive()) {
            target->takeDamage(damage);
        }
    } else {
        // Area of effect attack
        // Get primary target position
        sf::Vector2f targetPos = targets[0]->getPosition();
        
        // Apply damage to all targets in area of effect
        for (Enemy* enemy : targets) {
            if (enemy && enemy->isAlive()) {
                sf::Vector2f enemyPos = enemy->getPosition();
                float distance = std::sqrt(std::pow(enemyPos.x - targetPos.x, 2) + 
                                          std::pow(enemyPos.y - targetPos.y, 2));
                
                if (distance <= areaOfEffect) {
                    enemy->takeDamage(damage);
                }
            }
        }
    }
    
    // Update last attack time
    lastAttackTime = 0.0f; // This will be updated with current time in the update method
}

void InstantTower::update(float deltaTime) {
    // Call parent update first
    CombatTower::update(deltaTime);
    
    // Increment last attack time
    lastAttackTime += deltaTime;
    
    // Check if we can attack and have a target
    if (target && canAttack(lastAttackTime)) {
        std::vector<Enemy*> targets = {target};
        executeAction(targets);
        lastAttackTime = 0.0f;  // Reset attack timer
    }
}

bool InstantTower::canAttack(float currentTime) const {
    // Check if enough time has passed since last attack based on attack speed
    return (currentTime >= (1.0f / attackSpeed));
}
