#include "Entity/Tower/Projectile.hpp"

#include <Base/Constants.hpp>
#include <cmath>

#include "Entity/Enemy/Enemy.hpp"

// Constructor for entity-targeting projectile
Projectile::Projectile(Scene& scene) : Entity{scene} {}

void Projectile::update() {
    if (hitRemaining.getHealth() == 0) return;

    move();
    if (isCollided()) hitRemaining.takeDamage(hitRemaining.getHealth());
    return;
}

void Projectile::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    if (hasHit) return;
    target.draw(*sprite, state);
}

void Projectile::move() {
    if (projectileType == ProjectileType::TargetEntity && targetEntity &&
        targetEntity->isAlive() && hitRemaining.getHealth() > 0) {
        // Update velocity to track moving target
        sf::Vector2f newTargetPos = targetEntity->getPosition();
        sf::Vector2f direction = newTargetPos - position;
        if (direction.length() <
            1e-6)  // * Check for very small distance,
                   // * accomodated for floating pointer error
        {
            hitRemaining.takeDamage(hitRemaining.getHealth());
        }

        else {
            velocity = (direction.normalized()) * speed;
        }
    }
    setPosition(position + velocity * GameConstants::TICK_INTERVAL);
}

bool Projectile::isCollided() {
    if (projectileType == ProjectileType::TargetEntity) {
        if (!targetEntity || !targetEntity->isAlive()) {
            return true;  // Consider it a "hit" if target is dead (cleanup)
        }

        return isCollided(targetEntity->getPosition());
    }
    if (projectileType == ProjectileType::TargetLocation)
        return isCollided(targetLocation);
}

bool Projectile::isCollided(const sf::Vector2f& pos) const {
    float distance = (position - pos).length();

    return distance <= collisionDistance;
}

void Projectile::onDeath() {}