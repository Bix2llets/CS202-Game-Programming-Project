#include "Entity/Tower/Projectile/Projectile.hpp"

#include "Entity/Tower/Projectile/FlightMode.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/AreaEffect/AreaEffect.hpp"
#include "Scene/Level.hpp"
#include "Entity/Modules/SpriteAnimation.hpp"
#include "Entity/AreaEffect/AreaEffect.hpp"
#include "Entity/Factory/AreaEffectFactory.hpp"

#include "Utility/logger.hpp"
// For fallback unique id when not in a Level
#include <cstdint>

Projectile::Projectile(Scene& scene, const std::string id)
    : Entity(scene), levelRef(nullptr), id(id), type(ProjectileTargetType::Trajectory),
      pierceCount(1), currentPierceCount(0), collisionDistance(5.0f),
      speed(0.0f), velocity(0.0f, 0.0f), flying(true), source(nullptr),
      targetEntity(nullptr), flightMode(nullptr) 
{
    levelRef = dynamic_cast<Level*>(&scene);
    // Assign uniqueId only after levelRef is determined
    if (levelRef) {
        uniqueId = static_cast<int64_t>(levelRef->getRandom(RandomType::EntityID).nextU64());
    } else {
        uniqueId = static_cast<int64_t>(reinterpret_cast<std::uintptr_t>(this));
    }
}

Projectile::Projectile(const Projectile& other) 
    : Entity(other.scene), levelRef(other.levelRef), animation(other.animation), // Default construct animation
      id(other.id), type(other.type), rotateToTarget(other.rotateToTarget),
      pierceCount(other.pierceCount), currentPierceCount(other.currentPierceCount),
      collisionDistance(other.collisionDistance), speed(other.speed), velocity(other.velocity),
      flying(true), hitEnemies(), 
      targetEntity(nullptr), targetLocation(other.targetLocation), flightMode(other.flightMode),
      source(nullptr) // Initialize source to null - will be set by bindToTower call
{
    // Reset flying state and hit enemies for the new projectile instance
    flying = true;
    // After copying, assign a fresh uniqueId in the context of the new scene/level
    if (levelRef) {
        uniqueId = static_cast<int64_t>(levelRef->getRandom(RandomType::EntityID).nextU64());
    } else {
        uniqueId = static_cast<int64_t>(reinterpret_cast<std::uintptr_t>(this));
    }
}

void Projectile::update() {
    if(!flying) return; // If not flying, no need to update

    animation.update(); // Update animation frame
    
    if (targetEntity != nullptr) {
        if (!targetEntity->isAlive()) {
            stopFlying(); // Stop flying if target is dead
            return;
        }
        
        targetLocation = targetEntity->getPosition();
    }
    
    // Update projectile's flight mode behavior
    if (flightMode) {
        flightMode->update(this);
    }
    
    if (pierceCount == 0 && targetEntity) {
        if (targetEntity && isCollidedWith(targetEntity->getPosition())) {
            hitEnemies.push_back(targetEntity);
            targetEntity->onHit(stats.getStat(TowerStat::DAMAGE));
            targetEntity->applyEffects(sourceId, stats);
            stopFlying(); // Stop flying after hitting the target
        }
    } else {
        for (auto& enemy : levelRef->getEntityManager().getEnemies()) {
            if (hasHitEnemy(enemy)) continue; // Skip already hit enemies
            
            if (isCollidedWith(enemy->getPosition())) {
                hitEnemies.push_back(enemy); // Add to hit list
                enemy->onHit(stats.getStat(TowerStat::DAMAGE));
                enemy->applyEffects(sourceId, stats);
                increaseCurrentPierceCount();

                if(pierceCount - currentPierceCount <= 0 || enemy == targetEntity) {
                    stopFlying(); // Stop flying after hitting the target or if pierce count is exhausted
                    break; // Exit loop
                }
            }
        }
    }
    
    if (isCollidedWith(targetLocation)) {
        stopFlying(); // Stop flying if at target location
    }

    updateSpriteAnimation(); // Update sprite based on animation
}

void Projectile::stopFlying() {
    flying = false;
    hitEnemies.clear(); // Clear hit enemies

    // std::unique_ptr<AreaEffect> areaEffect = AreaEffectFactory::createFromConfigFile("explosion", *levelRef, sourceId);
    // areaEffect->setPosition(position);
    // areaEffect->setUp(&stats); // Set up area effect with projectile stats
    // levelRef->getEntityManager().addAreaEffect(std::move(areaEffect));
}

void Projectile::loadSpriteAnimation(const nlohmann::json& spriteAnimationPath) {
    animation.loadJson(spriteAnimationPath);
    // animation.updateSpriteSize(width, height);

    updateSpriteAnimation(); // Initialize sprite with the first frame
}

void Projectile::updateSpriteAnimation() {
    sprite = animation.getCurrentSprite();
    sprite.setPosition(position);
    sprite.setRotation(rotation);
}

void Projectile::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    target.draw(sprite, state);
}

inline bool Projectile::hasHitEnemy(Enemy* enemy) const {
    return std::find(hitEnemies.begin(), hitEnemies.end(), enemy) != hitEnemies.end();
}

inline bool Projectile::isCollidedWith(sf::Vector2f position) const {
    return (position - this->position).length() < collisionDistance;
}

void Projectile::bindToTower(Tower* tower) {
    if (!tower) {
        throw std::runtime_error("Cannot bind projectile to null tower");
    }

    source = tower;
    sourceId = tower->getUniqueId();
    stats = *tower->getStats() + tower->getUpgradeManager()->getAllUpgradeBonuses();
    speed = stats.getStat(TowerStat::PROJECTILE_SPEED, 1.0f);
    pierceCount = stats.getStat(TowerStat::PROJECTILE_PIERCE_COUNT, pierceCount);
}

void Projectile::setUpFlightMode() {
    if (!flightMode) {
        throw std::runtime_error("Flight mode is not set for projectile");
    }

    flightMode->setUp(this);
}

void Projectile::setTarget(Enemy* enemy) {
    if(type == ProjectileTargetType::TargetEntity) {
        targetEntity = enemy;
        if(enemy) targetEntity = enemy;
    } else if(type == ProjectileTargetType::TargetLocation) {
        if(enemy) targetLocation = enemy->getPosition();
    } else if(type == ProjectileTargetType::Trajectory) {
        if(enemy) targetLocation = enemy->getPosition();

        sf::Vector2f direction = (targetLocation - position).normalized();
        direction *= stats.getStat(TowerStat::PROJECTILE_RANGE, 300.0f);
        targetLocation = position + direction;
    } else {
        throw std::runtime_error("Invalid projectile target type for setting target");
    }

    if (flightMode) {
        flightMode->setUp(this); // Reinitialize flight mode with new target
    }
}

void Projectile::setPosition(const sf::Vector2f& pos) {
    position = pos;
    sprite.setPosition(position);
}

void Projectile::setRotation(const sf::Angle& rot) {
    rotation = rot;
    sprite.setRotation(rotation);
}
