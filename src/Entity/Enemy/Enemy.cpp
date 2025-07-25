#include "Entity/Enemy/Enemy.hpp"

#include <cmath>

#include "Base/Constants.hpp"
#include "Entity/Enemy/DyingState.hpp"
#include "Entity/Enemy/EnemyState.hpp"
#include "Entity/Enemy/MovingState.hpp"
#include "Entity/Modules/Effects/Effect.hpp"

#include "GUIComponents/EnemyPanel.hpp"

// Deep-copying copy constructor
Enemy::Enemy(const Enemy &other)
    : Entity(other),
      Damageable(other),
      currentState(other.currentState ? other.currentState->clone() : nullptr),
      health(other.health),
      enemyType(other.enemyType),
      reward(other.reward) {
    // Deep copy of state and health
    // If Enemy has any additional pointer or resource members, copy them here
}

void Enemy::update() {
    // Update effects first
    effects.update();
    
    // Handle periodic damage/healing from effects
    auto tickingEffects = effects.getTickingEffects();
    for (Effect* effect : tickingEffects) {
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
    path.setSpeedMultiplier(speedModifier);
    
    // if (currentState) {
    //     currentState->update(this);
    // }
    move();
    animation.update();
    healTimer.update();

    sprite = changeSpriteContent(sprite, animation.getCurrentSprite());
    sprite.setRotation(path.angleByVertical());
    while (healTimer.isAvailable()) {
        healTimer.use();
        health.heal(healAmount);
    }
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

    // Apply cumulative damage modifier from all effects
    damageModifier = damageModifier * effects.getDamageModifier(); 
    
    int finalDamage = static_cast<int>(damage * damageModifier);
    health.setHealth(health.getHealth() - finalDamage);
    
    // If enemy dies, change to dying state
    if (health.getHealth() <= 0) {
        changeState(std::make_unique<DyingState>());
    }
}

void Enemy::onDeath() {
    // Additional death handling could go here
    // e.g., play death sound, spawn particles, award points
    EnemyPanel::getInstance().clearEnemyIfReferencing(*this);
}

void Enemy::setPosition(const sf::Vector2f &position) {}
void Enemy::setRotation(const sf::Angle &angle) {}
void Enemy::onHeal(int healAmount) { health.heal(healAmount); }
bool Enemy::isAlive() { return health.getHealth() > 0
    && !path.isFinished(); 
}

Enemy::Enemy(Scene &scene) : Entity(scene) {}

sf::Sprite Enemy::changeSpriteContent(sf::Sprite current, sf::Sprite target) {
    auto rotation = current.getRotation();
    auto position = current.getPosition();
    current = target;
    current.setRotation(rotation);
    current.setPosition(position);
    return current;
}

Enemy::~Enemy() {
    onDeath();
}

void Enemy::applyEffect(EffectType type, EffectID id, int level, float duration) {
    auto effect = std::make_unique<Effect>(type, id, level, duration);
    effects.addEffect(std::move(effect));
}