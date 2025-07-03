
#pragma once

#include "Entity/Enemy/EnemyState.hpp"

/**
 * @brief State representing an enemy that is moving along a path
 */
class MovingState : public EnemyState {
public:
    /**
     * @brief Update the enemy's movement behavior
     * 
     * @param enemy The enemy being updated
     * @param deltaTime Time elapsed since last update
     */
    void update(Enemy* enemy) override;

    /**
     * @brief Called when entering the moving state
     * 
     * @param enemy The enemy entering this state
     */
    void enter(Enemy* enemy) override;

    /**
     * @brief Called when exiting the moving state
     * 
     * @param enemy The enemy exiting this state
     */
    void exit(Enemy* enemy) override;
};


