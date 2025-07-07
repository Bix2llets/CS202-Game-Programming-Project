#include "Entity/Enemy/EnemyState.hpp"
#include "Entity/Enemy/Enemy.hpp"

#include <cmath>

#include "Base/Constants.hpp"
#include "Entity/Enemy/DyingState.hpp"
#include "Entity/Enemy/MovingState.hpp"

// Deep-copying copy constructor
Enemy::Enemy(const Enemy& other)
    : Entity(other),
      Damageable(other),
      waypoints(other.waypoints),
      currentState(other.currentState ? other.currentState->clone() : nullptr),
      health(other.health),
      enemyType(other.enemyType),
      speed(other.speed),
      reward(other.reward)
{
    // Deep copy of state and health
    // If Enemy has any additional pointer or resource members, copy them here
}

void Enemy::update() {
    if (currentState) {
        currentState->update(this);
    }
    move();
}

void Enemy::draw(sf::RenderTarget &target, sf::RenderStates state) const {
    Entity::draw(target, state);
}

void Enemy::move() {
    if (waypoints->empty()) return;
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
    health.setHealth(health.getHealth() - damage);
    // If enemy dies, change to dying state
    if (getHealth() <= 0) {
        changeState(std::make_unique<DyingState>());
    }
}

void Enemy::onDeath() {
    // Additional death handling could go here
    // e.g., play death sound, spawn particles, award points
}

void Enemy::setPosition(const sf::Vector2f &position) {}
void Enemy::setRotation(const sf::Angle &angle) {}
void Enemy::heal(int healAmount) {}
int Enemy::getHealth() const {}
int Enemy::getMaxHealth() const {}
void Enemy::setHealth(int newHealth) {}
void Enemy::setMaxHealth(int newMaxHealth) {}
bool Enemy::isFullHealth() const {}
float Enemy::getHealthPercentage() const {}
bool Enemy::isAlive() {}