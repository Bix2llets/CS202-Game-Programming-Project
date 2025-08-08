#include "Entity/AreaEffect/AreaEffect.hpp"

#include <algorithm>

#include "Scene/Level.hpp"
#include "EntityManager.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Utility/logger.hpp"

AreaEffect::AreaEffect(Scene& scene) : Entity(scene) {
    levelRef = dynamic_cast<Level*>(&scene);
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
            enemy->onHit(stat.getStat(TowerStat::DAMAGE), type);
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
    animation.updateSpriteSize(stat.getStat(GeneralStat::WIDTH), stat.getStat(GeneralStat::HEIGHT));

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

void AreaEffect::setUp() {
    currentRepeats = 0;
    active = true;
    maxRepeats = static_cast<int>(stat.getStat(AreaEffectStat::REPEAT_TIMES));

    const float duration = stat.getStat(AreaEffectStat::DURATION);
    const float repeatInterval = stat.getStat(AreaEffectStat::REPEAT_INTERVAL);

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

    radius = stat.getStat(AreaEffectStat::RADIUS);
}

bool AreaEffect::isCollidedWith(const sf::Vector2f position) const {
    return (position - this->position).length() <= radius;
}