#include "Entity/Enemy/Enemy.hpp"

#include <cmath>

#include "Base/Constants.hpp"
#include "Entity/Enemy/DyingState.hpp"
#include "Entity/Enemy/EnemyState.hpp"
#include "Entity/Enemy/MovingState.hpp"
#include "Entity/Modules/Effects/Effect.hpp"
#include "GUIComponents/EnemyPanel.hpp"
#include "Utility/aligner.hpp"
#include "Utility/lerp.hpp"
#include "Utility/logger.hpp"

// Deep-copying copy constructor
Enemy::Enemy(const Enemy &other)
    : Entity(other),
      Damageable(other),
      currentState(other.currentState ? other.currentState->clone() : nullptr),
      health(other.health),
      enemyType(other.enemyType),
      petroleumReward(other.petroleumReward),
      scrapReward(other.scrapReward) {}

Enemy::Enemy(const Enemy &&other)
    : Entity(other),
      Damageable(other),
      currentState(other.currentState ? other.currentState->clone() : nullptr),
      health(other.health),
      enemyType(other.enemyType),
      petroleumReward(other.petroleumReward),
      scrapReward(other.scrapReward) {}

Enemy &Enemy::operator=(Enemy &other) {
    if (this != &other) {
        // Entity::operator=(other);
        Damageable::operator=(other);
        if (other.currentState) {
            currentState = other.currentState->clone();
        } else {
            currentState.reset();
        }
        health = other.health;
        enemyType = other.enemyType;
        petroleumReward = other.petroleumReward;
        scrapReward = other.scrapReward;
    }
    return *this;
};

Enemy &Enemy::operator=(Enemy &&other) {
    if (this != &other) {
        // Entity::operator=(std::move(other));
        Damageable::operator=(std::move(other));
        currentState = std::move(other.currentState);
        health = std::move(other.health);
        enemyType = std::move(other.enemyType);
        petroleumReward = std::move(other.petroleumReward);
        scrapReward = std::move(other.scrapReward);
    }
    return *this;
}

void Enemy::update() {
    // Update effects first
    effects.update();

    // Handle periodic damage/healing from effects

    auto tickingEffects = effects.getTickingEffects();
    for (Effect *effect : tickingEffects) {
        if (effect->shouldTick()) {
            switch (effect->getType()) {
                case EffectType::Burn:
                    this->onHit(effect->getLevel(), DamageType::Fire);
                    break;

                case EffectType::NapalmBurn:
                    this->onHit(effect->getLevel(), DamageType::Napalm);
                    break;

                case EffectType::Regeneration:
                    onHeal(effect->getLevel());
                    break;

                default:
                    break;
            }
            effect->resetTickTimer();
        }
    }

    // Check for death after effect damage
    if (health.getHealth() <= 0) {
        changeState(std::make_unique<DyingState>());
        return;
    }

    // Apply speed modifications to path
    float speedModifier = effects.getSpeedModifier();
    path.setEffectSpeedModifier(speedModifier);

    // if (currentState) {
    //     currentState->update(this);
    // }
    move();
    animation.update();
    attackDisplayTimer.update();

    sf::Color red = sf::Color::Red;
    sprite = changeSpriteContent(sprite, animation.getCurrentSprite());
    sprite.setColor(ColorMixer::perceptualLerp(
        red, sf::Color::White, attackDisplayTimer.getCompletionPercentage()));
    sprite.setRotation(path.angleByVertical());
}

void Enemy::move() {
    path.update();
    position = path.getPosition();
    sprite.setPosition(position);
}

void Enemy::draw(sf::RenderTarget &target, sf::RenderStates state) const {
    Entity::draw(target, state);
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

void Enemy::onHit(int damage, DamageType damageType) {
    // Apply damage modifiers from effects
    float damageModifier = effects.getDamageModifier();

    // Apply fire resistance for fire damage (but NOT napalm)
    if (damageType == DamageType::Fire) {
        float fireResistance = effects.getFireResistance();
        damageModifier *= (1.0f - fireResistance);
    }

    int finalDamage = static_cast<int>(damage * damageModifier);
    if (finalDamage > 0) attackDisplayTimer.reset();
    health.setHealth(health.getHealth() - finalDamage);

    // If enemy dies, change to dying state
    if (health.getHealth() <= 0) {
        changeState(std::make_unique<DyingState>());
    }
}

void Enemy::applyEffects(const EffectID effectId,
                         const EntityStat &damagerStats) {
    applyEffect(EffectType::Burn, damagerStats.getStat(EffectStat::BURN_LEVEL),
                damagerStats.getStat(EffectStat::BURN_DURATION), effectId);
    applyEffect(EffectType::NapalmBurn,
                damagerStats.getStat(EffectStat::NAPALM_BURN_LEVEL),
                damagerStats.getStat(EffectStat::NAPALM_BURN_DURATION),
                effectId);
    applyEffect(EffectType::Regeneration,
                damagerStats.getStat(EffectStat::REGENERATION_LEVEL),
                damagerStats.getStat(EffectStat::REGENERATION_DURATION),
                effectId);
    applyEffect(EffectType::Vulnerable,
                damagerStats.getStat(EffectStat::VULNERABLE_LEVEL),
                damagerStats.getStat(EffectStat::VULNERABLE_DURATION),
                effectId);
    applyEffect(EffectType::Resistance,
                damagerStats.getStat(EffectStat::RESISTANCE_LEVEL),
                damagerStats.getStat(EffectStat::RESISTANCE_DURATION),
                effectId);
    applyEffect(EffectType::FireResistance,
                damagerStats.getStat(EffectStat::FIRE_RESISTANCE_LEVEL),
                damagerStats.getStat(EffectStat::FIRE_RESISTANCE_DURATION),
                effectId);
    applyEffect(EffectType::Slow, damagerStats.getStat(EffectStat::SLOW_LEVEL),
                damagerStats.getStat(EffectStat::SLOW_DURATION), effectId);
    applyEffect(EffectType::Energized,
                damagerStats.getStat(EffectStat::ENERGIZED_LEVEL),
                damagerStats.getStat(EffectStat::ENERGIZED_DURATION), effectId);
}

void Enemy::onDeath() {
    // Additional death handling could go here
    // e.g., play death sound, spawn particles, award points
    EnemyPanel::getInstance().clearEnemyIfReferencing(*this);
}

void Enemy::setPosition(const sf::Vector2f &position) {}

void Enemy::setRotation(const sf::Angle &angle) {}

void Enemy::onHeal(int healAmount) { health.heal(healAmount); }

bool Enemy::isAlive() { return health.getHealth() > 0 && !path.isFinished(); }

Enemy::Enemy(Scene &scene) : Entity(scene) {}

sf::Sprite Enemy::changeSpriteContent(sf::Sprite current, sf::Sprite target) {
    auto rotation = current.getRotation();
    auto position = current.getPosition();
    current = target;
    current.setRotation(rotation);
    current.setPosition(position);
    return current;
}

Enemy::~Enemy() { onDeath(); }

void Enemy::applyEffect(EffectType type, int level, float duration,
                        EffectID id) {
    if (level <= 0 || duration <= 0) return;
    auto effect = std::make_unique<Effect>(type, level, duration, id);
    effects.addEffect(std::move(effect));
}