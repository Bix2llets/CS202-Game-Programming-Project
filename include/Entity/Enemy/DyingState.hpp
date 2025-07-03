#ifndef DYING_STATE_HPP
#define DYING_STATE_HPP

#include "Entity/Enemy/EnemyState.hpp"

/**
 * @brief State representing an enemy that is dying
 */
class DyingState : public EnemyState {
public:
    /**
     * @brief Update the enemy's dying behavior
     * 
     * @param enemy The enemy being updated
     * @param deltaTime Time elapsed since last update
     */
    void update(Enemy* enemy, float deltaTime) override;

    /**
     * @brief Called when entering the dying state
     * 
     * @param enemy The enemy entering this state
     */
    void enter(Enemy* enemy) override;

    /**
     * @brief Called when exiting the dying state
     * 
     * @param enemy The enemy exiting this state
     */
    void exit(Enemy* enemy) override;
};

#endif // DYING_STATE_HPP
