
/**
 * @file DyingState.hpp
 * @brief Declares the DyingState class for the enemy dying state.
 *
 * This state is used when an enemy is in the process of dying, handling death animation and cleanup.
 */
#pragma once

#include "Entity/Enemy/EnemyState.hpp"

/**
 * @class DyingState
 * @brief State representing an enemy that is dying.
 *
 * Handles the enemy's behavior, animation, and cleanup when dying.
 */
class DyingState : public EnemyState {
public:
    /**
     * @brief Update the enemy's dying behavior.
     * @param enemy The enemy being updated.
     */
    void update(Enemy* enemy) override;

    /**
     * @brief Called when entering the dying state.
     * @param enemy The enemy entering this state.
     */
    void enter(Enemy* enemy) override;

    /**
     * @brief Called when exiting the dying state.
     * @param enemy The enemy exiting this state.
     */
    void exit(Enemy* enemy) override;

    /**
     * @brief Clone this state (for deep copying).
     * @return A unique_ptr to a new copy of this state.
     */
    std::unique_ptr<EnemyState> clone() const override;
};


