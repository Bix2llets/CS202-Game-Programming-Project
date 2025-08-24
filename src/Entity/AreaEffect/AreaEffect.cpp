#include "Entity/AreaEffect/AreaEffect.hpp"

#include <algorithm>
// For fallback unique id when not in a Level
#include <cstdint>

#include "Scene/Level.hpp"
#include "EntityManager.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Utility/logger.hpp"

AreaEffect::AreaEffect(Scene& scene, uint64_t sourceId) : Entity(scene), sourceId(sourceId) {
    // Assign uniqueId only after levelRef is determined
    if (levelRef) {
        uniqueId = static_cast<int64_t>(levelRef->getRandom(RandomType::EntityID).nextU64());
    } else {
        uniqueId = static_cast<int64_t>(reinterpret_cast<std::uintptr_t>(this));
    }
}

AreaEffect::AreaEffect(const AreaEffect& other)
    : Entity(other.scene), animation(other.animation), // Default construct animation
      id(other.id), radius(other.radius), type(other.type),
      lifeTimer(other.lifeTimer), tickTimer(other.tickTimer),
      maxRepeats(other.maxRepeats), currentRepeats(0), active(true),
      sourceId(other.sourceId), stats(other.stats) 
{
    levelRef = other.levelRef;
    position = other.position;
    rotation = other.rotation;
    sprite = other.sprite;
    // After copying, assign a fresh uniqueId in the context of the new scene/level
    if (levelRef) {
        uniqueId = static_cast<int64_t>(levelRef->getRandom(RandomType::EntityID).nextU64());
    } else {
        uniqueId = static_cast<int64_t>(reinterpret_cast<std::uintptr_t>(this));
    }
}

void AreaEffect::setPosition(const sf::Vector2f& pos) {
    position = pos;
    sprite.setPosition(pos);
}

void AreaEffect::setRotation(const sf::Angle& rot) {
    rotation = rot;
    sprite.setRotation(rot);
}

void AreaEffect::applyTickEffect() {
    if (!levelRef) return;
    
    for (auto* enemy : levelRef->getEntityManager().getEnemies()) {
        if (!enemy || !enemy->isAlive()) continue;
        if (isCollidedWith(enemy->getPosition())) {
            enemy->onHit(stats.getStat(TowerStat::DAMAGE), type);
            enemy->applyEffects(uniqueId, stats);
        }
    }
}

void AreaEffect::updateSpriteAnimation() {
    sprite = animation.getCurrentSprite();
    sprite.setPosition(position);
    sprite.setRotation(rotation);
}

void AreaEffect::loadSpriteAnimation(const nlohmann::json& animationPath) {
    animation.loadJson(animationPath);
    animation.updateSpriteSize(stats.getStat(GeneralStat::WIDTH), stats.getStat(GeneralStat::HEIGHT));

    updateSpriteAnimation();
}

void AreaEffect::update() {
    if (!active) return;
    
    // Update timers
    lifeTimer.update();
    tickTimer.update();
    animation.update();
    
    // Apply periodic damage when available
    while (currentRepeats < maxRepeats && tickTimer.isAvailable()) {
        increaseCurrentRepeats();
        applyTickEffect();
        // In Continuous mode, consume one available tick without resetting the phase
        tickTimer.use();
    }

    // Expire when life is over
    if (lifeTimer.isAvailable()) {
        active = false;
    }

    updateSpriteAnimation(); // Update sprite based on animation
}

void AreaEffect::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    target.draw(sprite, state);
}

void AreaEffect::setUp(EntityStat* newStat) {
    currentRepeats = 0;
    active = true;
    maxRepeats = static_cast<int>(stats.getStat(AreaEffectStat::REPEAT_TIMES));

    const float duration = stats.getStat(AreaEffectStat::DURATION);
    const float repeatInterval = stats.getStat(AreaEffectStat::REPEAT_INTERVAL);

    // Configure life timer as single-shot with proper remaining time
    lifeTimer
        .setTimerMode(TimerMode::Single)
        .setTimeInterval(duration)
        .setRemainingTime(duration)
        .resume();

    // Configure tick timer as continuous; seed one immediate tick
    tickTimer
        .setTimerMode(TimerMode::Continuous)
        .setTimeInterval(repeatInterval)
        .setRemainingTime(repeatInterval)
        .increaseAvailable()
        .resume();

    radius = stats.getStat(AreaEffectStat::RADIUS);
}

bool AreaEffect::isCollidedWith(const sf::Vector2f position) const {
    return (position - this->position).length() <= radius;
}