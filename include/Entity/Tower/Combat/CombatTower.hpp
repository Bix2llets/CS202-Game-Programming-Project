#ifndef COMBAT_TOWER_HPP
#define COMBAT_TOWER_HPP

#include "Entity/Tower/Tower.hpp"
#include "Entity/Tower/Combat/TargetSelection.hpp"
#include <memory>
#include <vector>

// Forward declarations
class Enemy;

/**
 * @brief Combat Tower base class for towers that can attack enemies
 */
class CombatTower : public Tower {
protected:
    float range;
    int damage;
    float attackSpeed;
    Enemy* target;
    std::unique_ptr<Combat::TargetSelection> targetSelectionStrategy;
    float lastAttackTime;

public:
    /**
     * @brief Construct a new Combat Tower object
     * 
     * @param pos Position to place the tower
     * @param attackRange Attack range
     * @param attackDamage Damage per attack
     */
    CombatTower(const sf::Vector2f& pos = sf::Vector2f(0, 0),
                float attackRange = 150.0f, int attackDamage = 25);

    /**
     * @brief Destructor
     */
    ~CombatTower() override = default;

    /**
     * @brief Update the combat tower's behavior
     * 
     * @param deltaTime Time elapsed since last update
     */
    void update(float deltaTime) override;

    /**
     * @brief Execute combat action on selected targets
     * 
     * @param targets Vector of targets to act upon
     */
    virtual void executeAction(const std::vector<Enemy*>& targets) = 0;

    /**
     * @brief Find a target using the tower's target selection strategy
     * 
     * @param enemies Vector of available enemy targets
     * @return Enemy* Pointer to the selected target, or nullptr if none found
     */
    Enemy* selectTarget(const std::vector<Enemy*>& enemies);
    
    /**
     * @brief Set the target selection strategy for this tower
     * 
     * @param strategy New target selection strategy
     */
    void setTargetSelectionStrategy(std::unique_ptr<Combat::TargetSelection> strategy);

    /**
     * @brief Check if the tower can attack (cooldown check)
     * 
     * @param currentTime Current game time
     * @return bool True if can attack, false otherwise
     */
    bool canAttack(float currentTime) const;

    // Getters and setters
    float getRange() const { return range; }
    void setRange(float newRange) { range = newRange; }

    int getDamage() const { return damage; }
    void setDamage(int newDamage) { damage = newDamage; }

    float getAttackSpeed() const { return attackSpeed; }
    void setAttackSpeed(float newSpeed) { attackSpeed = newSpeed; }

    Enemy* getTarget() const { return target; }
    void setTarget(Enemy* newTarget) { target = newTarget; }
};

#endif // COMBAT_TOWER_HPP
