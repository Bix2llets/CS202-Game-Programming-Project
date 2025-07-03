
#pragma once

#include "Entity/Tower/Tower.hpp"

/**
 * @brief Passive Tower base class for towers that provide buffs or support
 * 
 * These towers don't directly attack enemies but provide beneficial effects
 * to nearby allied towers or debuffs to enemies.
 */
class PassiveTower : public Tower {
protected:
    float effectRange; // Range in which effects apply
    float effectPower; // Power/strength of the effect

public:
    /**
     * @brief Construct a new Passive Tower object
     * 
     * @param pos Position to place the tower
     * @param range Range of tower's effect
     * @param power Strength of the tower's effect
     */
    PassiveTower(const sf::Vector2f& pos = sf::Vector2f(0, 0),
                float range = 200.0f, float power = 1.0f);

    /**
     * @brief Destructor
     */
    ~PassiveTower() override = default;

    /**
     * @brief Update the passive tower's behavior
     */
    void update() override;

    // Getters and setters
    float getEffectRange() const { return effectRange; }
    void setEffectRange(float range) { effectRange = range; }
    
    float getEffectPower() const { return effectPower; }
    void setEffectPower(float power) { effectPower = power; }
};


