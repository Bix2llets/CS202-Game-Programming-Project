/**
 * @file Damageable.hpp
 * @brief Declares the Damageable interface for entities that can take damage and be healed.
 *
 * This interface defines the contract for entities that have health and can be damaged, such as enemies, towers, and player units.
 */
#pragma once

/**
 * @class Damageable
 * @brief Interface for entities that can take damage and be healed.
 *
 * Provides pure virtual methods for dealing and healing damage, querying and setting health, and handling death.
 */
class Damageable {
public:
    /**
     * @brief Deal damage to the entity.
     * @param damage Amount of damage to deal.
     */
    virtual void onHit(int damage) = 0;

    /**
     * @brief Heal the entity.
     * @param healAmount Amount of health to restore.
     */
    virtual void onHeal(int healAmount) = 0;

    /**
     * @brief Get the alive status of the entity.
     * @return bool True if alive, false otherwise.
     */
    virtual bool isAlive() = 0;

    /**
     * @brief Called when the entity's health reaches zero.
     *
     * Derived classes should override this to handle death behavior.
     */
    virtual void onDeath() = 0;
};
