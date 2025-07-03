#include "Entity/Enemy/DyingState.hpp"
#include "Entity/Enemy/Enemy.hpp"

void DyingState::update(Enemy* enemy, float deltaTime) {
    if (!enemy) return;
    
    // Dying behavior
    // Play death animations, handle death effects, etc.
    
    // In a real implementation, you might:
    // - Play death animation
    // - Spawn particles/effects
    // - Award player resources/score
    // - Remove enemy from game after animation completes
    
    // For now, just mark as not alive
    // The enemy will be cleaned up by the game management system
}

void DyingState::enter(Enemy* enemy) {
    if (!enemy) return;
    
    // Initialize dying state
    // Start death animation, play death sound, etc.
}

void DyingState::exit(Enemy* enemy) {
    if (!enemy) return;
    
    // Clean up dying state
    // This might not be called often since dying is usually a terminal state
}
