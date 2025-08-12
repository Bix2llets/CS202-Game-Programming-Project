
/**
 * @file Enemy.hpp
 * @brief Declares the Enemy class for enemy entities in the game.
 *
 * Enemy objects move along paths, can be attacked by towers, and use the State
 * pattern for AI behavior.
 */
#pragma once

#include <memory>
#include <vector>

#include "Entity/Damageable.hpp"
#include "Entity/Enemy/EnemyState.hpp"
#include "Entity/Entity.hpp"
#include "Entity/Health.hpp"
#include "Entity/Modules/EntityPath.hpp"
#include "Entity/Modules/SpriteAnimation.hpp"
#include "Entity/Modules/Timer.hpp"
#include "Entity/Modules/Effects/EntityEffect.hpp"
#include "Gameplay/Waypoint.hpp"
#include "Entity/Modules/EntityStat.hpp"

class Path;
class EnemyFactory;
class Scene;

/**
 * @enum EnemyType
 * @brief Enumeration for different enemy types.
 */
enum class EnemyType { Ground, Aerial };

/**
 * @class Enemy
 * @brief Enemy entity that moves along paths and can be attacked by towers.
 *
 * Uses the State pattern for AI behavior and supports health, movement, and
 * state transitions.
 *
 * This class must be constructed via enemy factory class, which require a json
 * object describing its stats. It may not and cannot be initiated outside of
 * the provided factory
 */
class Enemy : public Entity, public Damageable {
    friend class EnemyFactory;
    friend class EnemyPanel;

private:
    EntityPath path;
    SpriteAnimation animation;
    Health health;  ///< Health component
    EntityEffect effects;  ///< Effect management component
    Timer healTimer;

    Timer attackDisplayTimer;
    float healAmount;

    std::unique_ptr<EnemyState> currentState;  ///< Current AI state
    EnemyType enemyType;  ///< Type of enemy (ground, aerial, etc.)
    int petroleumReward;           ///< Reward for defeating this enemy
    int scrapReward;

    std::string name;

    /**
     * @brief Construct a new Enemy object (private, for factory use).
     * @param scene Reference to the scene this enemy belongs to.
     */
    Enemy(Scene& scene);

    sf::Sprite changeSpriteContent(sf::Sprite current, sf::Sprite target);

   public:
    /**
     * @brief Copy constructor (deep copy).
     * @param other Enemy to copy from.
     */
    Enemy(const Enemy& other);
    Enemy(const Enemy&& other);
    Enemy& operator= (Enemy& other);
    Enemy& operator= (Enemy&& other);

    /**
     * @brief Destructor.
     */
    ~Enemy() override;

    /**
     * @brief Update the enemy's state and behavior.
     * Calls the current state's update and moves the enemy.
     */
    void update() override;

    /**
     * @brief Render the enemy.
     * @param target Render target to draw on.
     * @param state Render states for drawing.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

    /**
     * @brief Move the enemy along its path.
     * Handles path following and position updates.
     */
    void move();

    /**
     * @brief Change the enemy's current state.
     * @param newState New state to transition to (ownership transferred).
     */
    void changeState(std::unique_ptr<EnemyState> newState);

    /**
     * @brief Take damage and handle death.
     * @param damage Amount of damage to take.
     * @param damageType Type of damage being dealt.
     */
    void onHit(int damage, DamageType damageType = DamageType::Physical) override;

    /**
     * @brief Handle damage and apply effects to this enemy based on the damager's stats.
     * @param damagerStats 
     */
    void onHitAndApplyEffects(const EffectID effectId, const EntityStat& damagerStats);

    /**
     * @brief Heal the enemy by a specified amount.
     * @param healAmount Amount of health to restore.
     */
    void onHeal(int healAmount) override;

    /**
     * @brief Check if the enemy is alive.
     * @return bool True if alive, false otherwise.
     */
    bool isAlive();

    /**
     * @brief Get the enemy's movement speed.
     * @return float Speed value.
     */
    float getSpeed() const { return path.getActualSpeed(); }

    /**
     * @brief Get the enemy's type.
     * @return EnemyType The type of this enemy.
     */
    EnemyType getEnemyType() const { return enemyType; }

    /**
     * @brief Get the reward for defeating this enemy.
     * @return int Reward value.
     */
    int getPetroleumReward() const { return petroleumReward; }
    int getScrapReward() const { return scrapReward; }

    /**
     * @brief Get the waypoints for this enemy's path.
     * @return Pointer to the vector of waypoints.
     */
    const std::vector<Waypoint>* getWaypoints() const {
        return path.getWaypoints();
    }

    /**
     * @brief Set the waypoints for this enemy's path.
     * @param newWaypoints Pointer to the new waypoints vector.
     */
    void setWaypoints(const std::vector<Waypoint>* newWaypoints) {
        path.setWaypoints(newWaypoints);
    }

    /**
     * @brief Get the current state object.
     * @return Pointer to the current EnemyState.
     */
    EnemyState* getCurrentState() const { return currentState.get(); }

    /**
     * @brief Set the position of the enemy.
     * @param pos New position.
     */
    void setPosition(const sf::Vector2f& pos) override;

    /**
     * @brief Set the rotation of the enemy.
     * @param rot New rotation.
     */
    void setRotation(const sf::Angle& rot) override;

    /**
     * @brief Get the current health
     * @return Current health
     */
    inline int getHealth() const { return health.getHealth(); }

    /**
     * @brief Apply an effect to this enemy.
     * If an effect with the same ID already exists, it will be refreshed.
     * @param type Type of effect to apply.
     * @param id Unique identifier for the effect source.
     * @param level Intensity/power of the effect.
     * @param duration Duration of the effect in seconds.
     */
    void applyEffect(EffectType type, EffectID id, int level, float duration);

    /**
     * @brief Get the effects manager for this enemy.
     * @return const EntityEffect& Reference to the effects manager.
     */
    const EntityEffect& getEffects() const { return effects; }

    inline bool isFinished() {return path.isFinished();}

    inline float getDistanceFromStart() const {
        return path.getDistanceFromStart();
    }
   protected:
    /**
     * @brief Called when the enemy dies.
     * Handles death logic, reward, and cleanup.
     */
    void onDeath() override;
};
