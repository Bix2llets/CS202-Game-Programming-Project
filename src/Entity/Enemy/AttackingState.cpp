#include "Entity/Enemy/AttackingState.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Enemy/MovingState.hpp"
#include "Entity/Enemy/DyingState.hpp"

void AttackingState::update(Enemy* enemy) {
    if (!enemy) return;
    
    // Attacking behavior
    // Look for targets to attack, execute attack animations, etc.
    
    // Example attacking logic:
    // - Find nearby player units or structures
    // - Deal damage to them
    // - Play attack animations
    
    // Check for state transitions
    // For example, if no more targets are available, return to moving
    // if (/* no targets available */) {
    //     enemy->changeState(std::make_unique<MovingState>());
    // }
    
    // If health drops to zero, transition to dying
    if (!enemy->isAlive()) {
        enemy->changeState(std::make_unique<DyingState>());
    }
}

void AttackingState::enter(Enemy* enemy) {
    if (!enemy) return;
    
    // Initialize attacking state
    // Set attack animations, reduce movement speed, etc.
}

void AttackingState::exit(Enemy* enemy) {
    if (!enemy) return;
    
    // Clean up attacking state
    // Stop attack animations, reset speed, etc.
}

std::unique_ptr<EnemyState> AttackingState::clone() const {
    return nullptr; // Cloning is not implemented, return nullptr
}
