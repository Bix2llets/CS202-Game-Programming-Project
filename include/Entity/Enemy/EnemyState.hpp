
#pragma once

// Forward declaration
class Enemy;

/**
 * @brief Base class for enemy states using the State pattern
 */
class EnemyState {
public:
    /**
     * @brief Virtual destructor
     */
    virtual ~EnemyState() = default;

    /**
     * @brief Update the enemy's behavior based on current state
     * 
     * @param enemy The enemy being updated
     * @param deltaTime Time elapsed since last update
     */
    virtual void update(Enemy* enemy) = 0;

    /**
     * @brief Called when entering this state
     * 
     * @param enemy The enemy entering this state
     */
    virtual void enter(Enemy* enemy) = 0;

    /**
     * @brief Called when exiting this state
     * 
     * @param enemy The enemy exiting this state
     */
    virtual void exit(Enemy* enemy) = 0;
};


