#include "Entity/Tower/Combat/CombatTower.hpp"
#include "Entity/Tower/Combat/TargetSelection.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include <cmath>

CombatTower::CombatTower(const sf::Vector2f& pos, float attackRange, int attackDamage)
    : Tower(pos), range(attackRange), damage(attackDamage), attackSpeed(1.0f),
      target(nullptr), lastAttackTime(0.0f) {
    
    // Set default target selection strategy to nearest enemy
    this->targetSelectionStrategy = std::make_unique<Combat::NearestTargetSelection>();
}

void CombatTower::update(float deltaTime) {
    // If tower has a target, check if it's still valid
    if (target) {
        if (!target->isAlive()) {
            target = nullptr;
        } else {
            // Calculate angle between tower and target for rotation
            sf::Vector2f targetPos = target->getPosition();
            sf::Vector2f direction = targetPos - position;
            
            // Calculate angle in degrees (SFML uses degrees)
            float angle = atan2(direction.y, direction.x) * 180.0f / 3.14159f;
            
            // Set the rotation of the upper part only (base doesn't rotate)
            setRotation(sf::degrees(angle));
        }
    }
    
    // Additional combat tower update logic would go here
    // For example, cooldown management, state changes, etc.
}

Enemy* CombatTower::selectTarget(const std::vector<Enemy*>& enemies) {
    if (this->targetSelectionStrategy) {
        target = this->targetSelectionStrategy->selectTarget(this, enemies);
        return target;
    }
    return nullptr;
}

void CombatTower::setTargetSelectionStrategy(std::unique_ptr<Combat::TargetSelection> strategy) {
    this->targetSelectionStrategy = std::move(strategy);
}

bool CombatTower::canAttack(float currentTime) const {
    return (currentTime - lastAttackTime) >= (1.0f / attackSpeed);
}
