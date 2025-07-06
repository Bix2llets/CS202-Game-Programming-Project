
#pragma once

#include <memory>
#include <vector>

#include "Entity/Damageable.hpp"
#include "Entity/Enemy/EnemyState.hpp"
#include "Entity/Entity.hpp"

// Forward declarations
class EnemyState;
class Waypoint;
class Map;
class EnemyFactory;

/**
 * @brief Enumeration for different enemy types
 */
enum class EnemyType { Ground, Aerial };

/**
 * @brief Enemy entity that moves along paths and can be attacked by towers
 */
class Enemy : public Entity, public Damageable {
    friend class EnemyFactory;

   private:
    const std::vector<sf::Vector2f>* waypoints;
    std::unique_ptr<EnemyState> currentState;
    EnemyType enemyType;
    float speed;
    int reward;

   public:
    /**
     * @brief Construct a new Enemy object
     *
     * @param pos Initial position
     * @param hp Health points
     * @param moveSpeed Movement speed
     * @param type Type of enemy
     */
    Enemy(const sf::Vector2f& pos = sf::Vector2f(0, 0), int hp = 50,
          float moveSpeed = 100.0f, EnemyType type = EnemyType::Ground);

    /**
     * @brief Destructor
     */
    ~Enemy() override = default;

    /**
     * @brief Update the enemy's state and behavior
     *
     * @param deltaTime Time elapsed since last update
     */
    void update() override;

    /**
     * @brief Render the enemy
     *
     * @param target Render target to draw on
     */
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

    /**
     * @brief Move the enemy along its path
     *
     * @param deltaTime Time elapsed since last update
     */
    void move();

    /**
     * @brief Change the enemy's current state
     *
     * @param newState New state to transition to
     */
    void changeState(std::unique_ptr<EnemyState> newState);

    /**
     * @brief Take damage and handle death
     *
     * @param damage Amount of damage to take
     */
    void takeDamage(int damage) override;

    // Getters and setters
    float getSpeed() const { return speed; }

    EnemyType getEnemyType() const { return enemyType; }

    int getReward() const { return reward; }

    const std::vector<sf::Vector2f>* getWaypoints() const { return waypoints; }
    void setWaypoints(const std::vector<sf::Vector2f>* newWaypoints) {
        waypoints = newWaypoints;
    }

    EnemyState* getCurrentState() const { return currentState.get(); }

   protected:
    /**
     * @brief Called when the enemy dies
     */
    void onDeath() override;
};
