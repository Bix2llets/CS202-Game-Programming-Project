#include "Entity/Enemy/MovingState.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Enemy/AttackingState.hpp"

void MovingState::update(Enemy* enemy) {
    if (!enemy) return;
    
    // Move the enemy along its path
    enemy->move();
    
    // Check for state transitions
    // For example, if enemy reaches end of path or encounters an obstacle
    // This is where you would implement logic to transition to attacking state
    // if the enemy encounters a player unit or structure to attack
    
    // Example transition logic (simplified):
    // if (/* condition to start attacking */) {
    //     enemy->changeState(std::make_unique<AttackingState>());
    // }
}

void MovingState::enter(Enemy* enemy) {
    if (!enemy) return;
    
    // Initialize moving state
    // Set appropriate animation, speed modifiers, etc.
}

void MovingState::exit(Enemy* enemy) {
    if (!enemy) return;
    
    // Clean up moving state
    // Stop movement animations, etc.
}

std::unique_ptr<EnemyState> MovingState::clone() const {
    return nullptr; // Not implemented, return nullptr
}