#include "Entity/Tower/Combat/ContinuousTower.hpp"
#include "Entity/Tower/Combat/TargetSelection.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include <iostream>

ContinuousTower::ContinuousTower(const sf::Vector2f& pos, float attackRange, int attackDamage,
                               float chargeUp, float chargeDown, float threshold, float width)
    : CombatTower(pos, attackRange, attackDamage), 
      charge(0.0f), chargeUpRate(chargeUp), chargeDownRate(chargeDown),
      chargeThreshold(threshold), beamWidth(width), isActive(false) {
    // Initialize continuous tower specific properties
}

void ContinuousTower::executeAction(const std::vector<Enemy*>& targets) {
    if (targets.empty()) return;
    
    Enemy* mainTarget = targets[0];
    if (mainTarget && mainTarget->isAlive()) {
        // Apply damage to the target
        mainTarget->takeDamage(damage * (charge / chargeThreshold));
        
        // Set tower to active state (for visuals)
        isActive = true;
        
        // In a full implementation, we would also create a beam effect here
        std::cout << "Continuous beam attacking enemy with intensity: " 
                  << (charge / chargeThreshold) << std::endl;
    }
}

void ContinuousTower::update(float deltaTime) {
    // Call parent update first to update position and rotation
    CombatTower::update(deltaTime);
    
    // Update charge based on target presence
    if (target && target->isAlive()) {
        // Increase charge when we have a target
        charge = std::min(charge + (chargeUpRate * deltaTime), 1.5f);
    } else {
        // Decrease charge when no target
        charge = std::max(charge - (chargeDownRate * deltaTime), 0.0f);
        isActive = false;
    }
    
    // If charge is above threshold and we have a target, attack
    if (charge >= chargeThreshold && target && target->isAlive()) {
        std::vector<Enemy*> targets = {target};
        executeAction(targets);
    } else {
        isActive = false;
    }
}
