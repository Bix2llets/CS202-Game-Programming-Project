#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Enemy/MovingState.hpp"
#include "Entity/Enemy/DyingState.hpp"
#include <cmath>

Enemy::Enemy(const sf::Vector2f& pos, int hp, float moveSpeed, EnemyType type)
    : Entity(pos), Damageable(hp), speed(moveSpeed), enemyType(type), reward(10) {
    
    // Initialize with moving state
    currentState = std::make_unique<MovingState>();
    currentState->enter(this);
    
    // Set reward based on enemy type
    switch (enemyType) {
        case EnemyType::Basic:
            reward = 10;
            break;
        case EnemyType::Fast:
            reward = 15;
            break;
        case EnemyType::Heavy:
            reward = 25;
            break;
        case EnemyType::Flying:
            reward = 20;
            break;
    }
}

void Enemy::update(float deltaTime) {
    if (currentState) {
        currentState->update(this, deltaTime);
    }
}

void Enemy::render(sf::RenderTarget& target) {
    Entity::render(target);
    
    // Additional rendering for enemy-specific elements could go here
    // For example, health bars, status effects, etc.
}

void Enemy::move(float deltaTime) {
    if (waypoints.empty()) return;
    
    // Simple movement towards next waypoint
    // This is a basic implementation - could be enhanced with pathfinding
    if (!waypoints.empty()) {
        sf::Vector2f targetPos = waypoints[0];
        sf::Vector2f direction = targetPos - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (distance > 5.0f) {
            // Normalize direction and apply speed
            direction /= distance;
            sf::Vector2f movement = direction * speed * deltaTime;
            setPosition(position + movement);
        } else {
            // Reached waypoint, remove it
            waypoints.erase(waypoints.begin());
        }
    }
}

void Enemy::changeState(std::unique_ptr<EnemyState> newState) {
    if (currentState) {
        currentState->exit(this);
    }
    
    currentState = std::move(newState);
    
    if (currentState) {
        currentState->enter(this);
    }
}

void Enemy::takeDamage(int damage) {
    Damageable::takeDamage(damage);
    
    // If enemy dies, change to dying state
    if (getHealth() <= 0) {
        changeState(std::make_unique<DyingState>());
    }
}

void Enemy::onDeath() {
    setAlive(false);
    // Additional death handling could go here
    // e.g., play death sound, spawn particles, award points
}
