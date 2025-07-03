#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity/Entity.hpp"
#include "Entity/Damageable.hpp"
#include "Entity/Enemy/EnemyState.hpp"
#include <vector>
#include <memory>

// Forward declarations
class EnemyState;

/**
 * @brief Enumeration for different enemy types
 */
enum class EnemyType {
    Basic,
    Fast,
    Heavy,
    Flying
};

/**
 * @brief Enemy entity that moves along paths and can be attacked by towers
 */
class Enemy : public Entity, public Damageable {
private:
    float speed;
    std::vector<int> pathIndices;
    std::vector<sf::Vector2f> waypoints;
    EnemyType enemyType;
    int reward;
    std::unique_ptr<EnemyState> currentState;

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
          float moveSpeed = 100.0f, EnemyType type = EnemyType::Basic);

    /**
     * @brief Destructor
     */
    ~Enemy() override = default;

    /**
     * @brief Update the enemy's state and behavior
     * 
     * @param deltaTime Time elapsed since last update
     */
    void update(float deltaTime) override;

    /**
     * @brief Render the enemy
     * 
     * @param target Render target to draw on
     */
    void render(sf::RenderTarget& target) override;

    /**
     * @brief Move the enemy along its path
     * 
     * @param deltaTime Time elapsed since last update
     */
    void move(float deltaTime);

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
    void setSpeed(float newSpeed) { speed = newSpeed; }

    EnemyType getEnemyType() const { return enemyType; }
    void setEnemyType(EnemyType type) { enemyType = type; }

    int getReward() const { return reward; }
    void setReward(int newReward) { reward = newReward; }

    const std::vector<sf::Vector2f>& getWaypoints() const { return waypoints; }
    void setWaypoints(const std::vector<sf::Vector2f>& newWaypoints) { waypoints = newWaypoints; }

    EnemyState* getCurrentState() const { return currentState.get(); }

protected:
    /**
     * @brief Called when the enemy dies
     */
    void onDeath() override;
};

#endif // ENEMY_HPP
