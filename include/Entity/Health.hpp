
/**
 * @file Health.hpp
 * @brief Declares the Health class for managing entity health and related
 * operations.
 *
 * The Health class provides a simple interface for tracking and modifying an
 * entity's health. It supports taking damage, healing, and querying health
 * ratios. This class is intended to be used as a component or member within
 * entities that require health management.
 */
#pragma once

/**
 * @class Health
 * @brief Encapsulates health management for game entities.
 *
 * Provides methods to take damage, heal, and query or set health/max health
 * values.
 */
class Health {
   private:
    float maxHealth;      ///< Maximum health value
    float currentHealth;  ///< Current health value

   public:
    /**
     * @brief Reduces health by the specified amount.
     * @param amount Amount of damage to apply.
     */
    void takeDamage(float amount);

    /**
     * @brief Increases health by the specified amount, up to maxHealth.
     * @param amount Amount of health to restore.
     */
    void heal(float amount);

    /**
     * @brief Returns the ratio of current health to max health (0.0 to 1.0).
     * @return float Health ratio.
     */
    float getHealthToMaxHealthRatio();

    /**
     * @brief Sets the current health value.
     * @param amount New health value.
     */
    void setHealth(float amount);

    /**
     * @brief Gets the current health value.
     * @return float Current health.
     */
    float getHealth() const;

    /**
     * @brief Sets the maximum health value.
     * @param amount New maximum health value.
     */
    void setMaxHealth(float amount);

    /**
     * @brief Gets the maximum health value.
     * @return float Maximum health.
     */
    float getMaxHealth() const;

   private:
    /**
     * @brief Clamp the current health to the range [0, maxHealth].
     * Ensures health never drops below zero or exceeds the maximum.
     */
    void clampHealth();
};