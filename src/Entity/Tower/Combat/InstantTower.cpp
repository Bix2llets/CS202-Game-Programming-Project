#include "Entity/Tower/Combat/InstantTower.hpp"
#include "Entity/Tower/Combat/TargetSelection.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include <cmath>

InstantTower::InstantTower(const sf::Vector2f& pos, float attackRange, int attackDamage, 
                         float attackSpeed, float aoe)
    : CombatTower(pos, attackRange, attackDamage), 
      areaOfEffect(aoe){
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
    resetRemainingTime(); // This will be updated with current time in the update method
}

void InstantTower::update() {
    // Call parent update first
    CombatTower::update();
    
    // Increment last attack time
    // Check if we can attack and have a target
    if (target && canAttack()) {
        std::vector<Enemy*> targets = {target};
        executeAction(targets);
        resetRemainingTime();  // Reset attack timer
    }
}

