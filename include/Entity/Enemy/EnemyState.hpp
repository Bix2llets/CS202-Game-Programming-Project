
/**
 * @file EnemyState.hpp
 * @brief Declares the EnemyState base class for enemy AI states (State pattern).
 *
 * Provides the interface for all enemy states, including update, enter, exit, and cloning.
 */
#pragma once
#include <memory>
// Forward declaration
class Enemy;

/**
 * @class EnemyState
 * @brief Base class for enemy states using the State pattern.
 *
 * All concrete enemy states (e.g., MovingState, AttackingState, DyingState) should inherit from this class.
 */
class EnemyState {
public:
    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    virtual ~EnemyState() = default;

    /**
     * @brief Clone this state (for deep copying).
     * @return A unique_ptr to a new copy of this state.
     */
    virtual std::unique_ptr<EnemyState> clone() const = 0;

    /**
     * @brief Update the enemy's behavior based on current state.
     * @param enemy The enemy being updated.
     */
    virtual void update(Enemy* enemy) = 0;

    /**
     * @brief Called when entering this state.
     * @param enemy The enemy entering this state.
     */
    virtual void enter(Enemy* enemy) = 0;

    /**
     * @brief Called when exiting this state.
     * @param enemy The enemy exiting this state.
     */
    virtual void exit(Enemy* enemy) = 0;
};


