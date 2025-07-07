
/**
 * @file MovingState.hpp
 * @brief Declares the MovingState class for the enemy moving state.
 *
 * This state is used when an enemy is moving along its path.
 */
#pragma once

#include "Entity/Enemy/EnemyState.hpp"

/**
 * @class MovingState
 * @brief State representing an enemy that is moving along a path.
 *
 * Handles the enemy's movement logic and path following.
 */
class MovingState : public EnemyState {
public:
    /**
     * @brief Update the enemy's movement behavior.
     * @param enemy The enemy being updated.
     */
    void update(Enemy* enemy) override;

    /**
     * @brief Called when entering the moving state.
     * @param enemy The enemy entering this state.
     */
    void enter(Enemy* enemy) override;

    /**
     * @brief Called when exiting the moving state.
     * @param enemy The enemy exiting this state.
     */
    void exit(Enemy* enemy) override;

    /**
     * @brief Clone this state (for deep copying).
     * @return A unique_ptr to a new copy of this state.
     */
    std::unique_ptr<EnemyState> clone() const;
};


