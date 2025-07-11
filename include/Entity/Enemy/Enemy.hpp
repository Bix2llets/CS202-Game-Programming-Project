
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
#include "Gameplay/Waypoint.hpp"
class Map;
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

   private:
    EntityPath path;
    SpriteAnimation animation;
    Health health;  ///< Health component
    Timer healTimer;
    float healAmount;

    std::unique_ptr<EnemyState> currentState;  ///< Current AI state
    EnemyType enemyType;  ///< Type of enemy (ground, aerial, etc.)
    int reward;           ///< Reward for defeating this enemy

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

    /**
     * @brief Destructor.
     */
    ~Enemy() override = default;

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
     */
    void onHit(int damage) override;

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
    int getReward() const { return reward; }

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

   protected:
    /**
     * @brief Called when the enemy dies.
     * Handles death logic, reward, and cleanup.
     */
    void onDeath() override;
};
