
#pragma once

#include "Entity/Tower/Combat/CombatTower.hpp"
#include "Entity/Tower/Combat/TargetSelection.hpp"

/**
 * @brief Tower that applies continuous damage to targets
 * 
 * This tower deals continuous damage over time rather than discrete attacks.
 * Examples include laser towers, flame throwers, and tesla coils.
 */
class ContinuousTower : public CombatTower {
private:
    float charge;          // Current charge level (0.0 to 1.0+)
    float chargeUpRate;    // How fast the tower charges up when targeting an enemy
    float chargeDownRate;  // How fast the tower charge decreases when no target
    float chargeThreshold; // Threshold at which the tower can attack
    float beamWidth;       // Width of the damage beam/ray (visual)
    bool isActive;         // Whether the tower is currently applying damage

public:
    /**
     * @brief Construct a new Continuous Tower object
     * 
     * @param pos Position to place the tower
     * @param attackRange Attack range
     * @param attackDamage Damage per second
     * @param chargeUpRate How fast the tower charges when targeting enemy (per second)
     * @param chargeDownRate How fast the tower loses charge when not targeting (per second)
     * @param chargeThreshold Minimum charge required to attack
     * @param width Width of the beam/ray
     */
    ContinuousTower(const sf::Vector2f& pos = sf::Vector2f(0, 0),
                   float attackRange = 120.0f, int attackDamage = 15,
                   float chargeUpRate = 0.5f, float chargeDownRate = 0.3f,
                   float chargeThreshold = 0.8f, float width = 3.0f);

    /**
     * @brief Destructor
     */
    ~ContinuousTower() override = default;

    /**
     * @brief Apply continuous damage to targets
     * 
     * @param targets Vector of targets to damage
     */
    void executeAction(const std::vector<Enemy*>& targets) override;
    
    /**
     * @brief Update with special handling for continuous damage
     * 
     * @param deltaTime Time elapsed since last update
     */
    void update() override;
    
    // Getters and setters
    float getCharge() const { return charge; }
    void setCharge(float newCharge) { charge = newCharge; }
    
    float getChargeUpRate() const { return chargeUpRate; }
    void setChargeUpRate(float rate) { chargeUpRate = rate; }
    
    float getChargeDownRate() const { return chargeDownRate; }
    void setChargeDownRate(float rate) { chargeDownRate = rate; }
    
    float getChargeThreshold() const { return chargeThreshold; }
    void setChargeThreshold(float threshold) { chargeThreshold = threshold; }
    
    float getBeamWidth() const { return beamWidth; }
    void setBeamWidth(float width) { beamWidth = width; }
    
    bool getIsActive() const { return isActive; }
    void setIsActive(bool active) { isActive = active; }
};


