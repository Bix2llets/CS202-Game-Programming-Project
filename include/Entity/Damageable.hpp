#pragma once

/**
 * @brief Interface for entities that can take damage
 *
 * This interface defines the contract for entities that have health
 * and can be damaged, such as enemies, towers, and player units.
 */
class Damageable {
   public:
    /**
     * @brief Deal damage to the entity
     *
     * @param damage Amount of damage to deal
     */
    virtual void takeDamage(int damage) = 0;

    /**
     * @brief Heal the entity
     *
     * @param healAmount Amount of health to restore
     */
    virtual void heal(int healAmount) = 0;

    /**
     * @brief Get the entity's current health
     *
     * @return int Current health points
     */
    virtual int getHealth() const = 0;

    /**
     * @brief Get the entity's maximum health
     *
     * @return int Maximum health points
     */
    virtual int getMaxHealth() = 0;

    /**
     * @brief Set the maximum health
     *
     * @param newMaxHealth New maximum health value
     */
    virtual void setMaxHealth(int newMaxHealth) = 0;
    /**
     * @brief Check if the entity is at full health
     *
     * @return bool True if at full health, false otherwise
     */
    virtual bool isFullHealth() const = 0;

    /**
     * @brief Get health as a percentage
     *
     * @return float Health percentage (0.0 to 1.0)
     */
    virtual float getHealthPercentage() const = 0;

    /**
     * @brief Get the alive status
     *
     */
    virtual bool isAlive() = 0;

   protected:
    /**
     * @brief Called when the entity's health reaches zero
     *
     * Derived classes should override this to handle death behavior
     */
    virtual void onDeath() = 0;
};
