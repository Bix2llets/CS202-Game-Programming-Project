#ifndef INSTANT_TOWER_HPP
#define INSTANT_TOWER_HPP

#include "Entity/Tower/Combat/CombatTower.hpp"
#include "Entity/Tower/Combat/TargetSelection.hpp"

/**
 * @brief Tower that deals instant damage to targets
 * 
 * This tower applies damage instantly to enemies without projectiles or animations.
 * Examples include lightning towers or magic towers with instant effects.
 */
class InstantTower : public CombatTower {
private:
    float areaOfEffect;    // Radius for potential area damage, 0 means single target
    float attackSpeed;     // Attacks per second
    float lastAttackTime;  // Time of last attack

public:
    /**
     * @brief Construct a new Instant Tower object
     * 
     * @param pos Position to place the tower
     * @param attackRange Attack range
     * @param attackDamage Damage per attack
     * @param attackSpeed Attacks per second
     * @param aoe Area of effect radius (0 for single target)
     */
    InstantTower(const sf::Vector2f& pos = sf::Vector2f(0, 0),
                 float attackRange = 150.0f, int attackDamage = 25,
                 float attackSpeed = 1.0f, float aoe = 0.0f);

    /**
     * @brief Destructor
     */
    ~InstantTower() override = default;

    /**
     * @brief Execute instant damage action on targets
     * 
     * @param targets Vector of targets to damage
     */
    void executeAction(const std::vector<Enemy*>& targets) override;
    
    /**
     * @brief Update the instant tower's behavior
     * 
     * @param deltaTime Time elapsed since last update
     */
    void update(float deltaTime) override;
    
    /**
     * @brief Check if the tower can attack based on attack speed
     * 
     * @param currentTime Current game time
     * @return bool True if the tower can attack, false otherwise
     */
    bool canAttack(float currentTime) const;
    
    // Getters and setters
    float getAreaOfEffect() const { return areaOfEffect; }
    void setAreaOfEffect(float aoe) { areaOfEffect = aoe; }
    
    float getAttackSpeed() const { return attackSpeed; }
    void setAttackSpeed(float speed) { attackSpeed = speed; }
};

#endif // INSTANT_TOWER_HPP
