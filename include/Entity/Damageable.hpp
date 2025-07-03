#ifndef DAMAGEABLE_HPP
#define DAMAGEABLE_HPP

/**
 * @brief Interface for entities that can take damage
 * 
 * This interface defines the contract for entities that have health
 * and can be damaged, such as enemies, towers, and player units.
 */
class Damageable {
protected:
    int health;
    int maxHealth;

public:
    /**
     * @brief Construct a new Damageable object
     * 
     * @param hp Initial health points
     */
    Damageable(int hp = 100) : health(hp), maxHealth(hp) {}

    /**
     * @brief Virtual destructor
     */
    virtual ~Damageable() = default;

    /**
     * @brief Deal damage to the entity
     * 
     * @param damage Amount of damage to deal
     */
    virtual void takeDamage(int damage) {
        if (health > 0) {
            health -= damage;
            if (health <= 0) {
                health = 0;
                onDeath();
            }
        }
    }

    /**
     * @brief Heal the entity
     * 
     * @param healAmount Amount of health to restore
     */
    virtual void heal(int healAmount) {
        health += healAmount;
        if (health > maxHealth) {
            health = maxHealth;
        }
    }

    /**
     * @brief Get the entity's current health
     * 
     * @return int Current health points
     */
    int getHealth() const { return health; }

    /**
     * @brief Get the entity's maximum health
     * 
     * @return int Maximum health points
     */
    int getMaxHealth() const { return maxHealth; }

    /**
     * @brief Set the maximum health
     * 
     * @param newMaxHealth New maximum health value
     */
    void setMaxHealth(int newMaxHealth) { 
        maxHealth = newMaxHealth; 
        if (health > maxHealth) {
            health = maxHealth;
        }
    }

    /**
     * @brief Check if the entity is at full health
     * 
     * @return bool True if at full health, false otherwise
     */
    bool isFullHealth() const { return health == maxHealth; }

    /**
     * @brief Get health as a percentage
     * 
     * @return float Health percentage (0.0 to 1.0)
     */
    float getHealthPercentage() const {
        return maxHealth > 0 ? static_cast<float>(health) / maxHealth : 0.0f;
    }

protected:
    /**
     * @brief Called when the entity's health reaches zero
     * 
     * Derived classes should override this to handle death behavior
     */
    virtual void onDeath() = 0;
};

#endif // DAMAGEABLE_HPP
