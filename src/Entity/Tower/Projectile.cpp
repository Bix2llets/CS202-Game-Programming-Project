#include "Entity/Tower/Projectile.hpp"

#include <Base/Constants.hpp>
#include <cmath>

#include "Entity/Enemy/Enemy.hpp"

// Constructor for entity-targeting projectile
Projectile::Projectile(const sf::Vector2f& pos, Enemy* targetEnemy,
                       int projectileDamage, float projectileSpeed)
    : Entity(pos),
      damage(projectileDamage),
      targetEntity(targetEnemy),
      speed(projectileSpeed),
      hasHit(false),
      projectileType(ProjectileType::TargetEntity) {
    if (targetEntity) {
        // Calculate initial velocity towards target
        targetLocation = targetEntity->getPosition();
        sf::Vector2f direction = targetLocation - position;
        float distance =
            std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > 0) {
            velocity = (direction / distance) * speed;
        }
    }
}

// Constructor for location-targeting projectile
Projectile::Projectile(const sf::Vector2f& pos, const sf::Vector2f& targetPos,
                       int projectileDamage, float projectileSpeed)
    : Entity(pos),
      damage(projectileDamage),
      targetEntity(nullptr),
      targetLocation(targetPos),
      speed(projectileSpeed),
      hasHit(false),
      projectileType(ProjectileType::TargetLocation) {
    // Calculate velocity towards target location
    sf::Vector2f direction = targetLocation - position;
    float distance =
        std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0) {
        velocity = (direction / distance) * speed;
    }
}

void Projectile::update() {
    if (!hasHit) {
        move();

        if (isCollided()) {
            // Handle collision based on projectile type
            if (projectileType == ProjectileType::TargetEntity &&
                targetEntity && targetEntity->isAlive()) {
                targetEntity->takeDamage(damage);
            }
            // For location targeting, we just mark as hit when reaching the
            // location

            hasHit = true;
            takeDamage(getHealth());  // Projectile is destroyed after hitting
        }
    } else {
        takeDamage(
            getHealth());  // Ensure projectile is destroyed if already hit
    }
}

void Projectile::draw(sf::RenderTarget &target, sf::RenderStates state) const {
    if (hasHit) return;
    target.draw(*sprite, state);
}

void Projectile::move() {
    if (projectileType == ProjectileType::TargetEntity && targetEntity &&
        targetEntity->isAlive()) {
        // Update velocity to track moving target
        sf::Vector2f newTargetPos = targetEntity->getPosition();
        sf::Vector2f direction = newTargetPos - position;
        float distance =
            std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > 0) {
            velocity = (direction / distance) * speed;
        }
    }
    setPosition(position + velocity * GameConstants::TICK_INTERVAL);
}

bool Projectile::isCollided() {
    if (projectileType == ProjectileType::TargetEntity) {
        if (!targetEntity || !targetEntity->isAlive()) {
            return true;  // Consider it a "hit" if target is dead (cleanup)
        }

        return isCollidedWith(targetEntity->getPosition());
    }
    if (projectileType == ProjectileType::TargetLocation)
        return isCollidedWith(targetLocation);
}

bool Projectile::isCollidedWith(const sf::Vector2f& pos) const {
    float distance = std::sqrt(std::pow(pos.x - position.x, 2) +
                               std::pow(pos.y - position.y, 2));

    return distance <= COLLISION_DISTANCE;
}

void Projectile::onDeath() {}