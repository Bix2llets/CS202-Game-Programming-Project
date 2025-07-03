#ifndef PROJECTILE_TOWER_HPP
#define PROJECTILE_TOWER_HPP

#include "Entity/Tower/Combat/CombatTower.hpp"
#include "Entity/Tower/Combat/TargetSelection.hpp"

/**
 * @brief Tower that fires projectiles at enemies
 * 
 * This tower launches projectiles that travel to the target before dealing damage.
 * Examples include arrow towers, cannon towers, and missile launchers.
 */
class ProjectileTower : public CombatTower {
private:
    float projectileSpeed;
    float splashRadius;    // Radius for splash damage, 0 means no splash
    float attackSpeed;     // Attacks per second
    float lastAttackTime;  // Time of last attack

public:
    /**
     * @brief Construct a new Projectile Tower object
     * 
     * @param pos Position to place the tower
     * @param attackRange Attack range
     * @param attackDamage Damage per attack
     * @param attackSpeed Attacks per second
     * @param projSpeed Projectile travel speed
     * @param splash Splash damage radius (0 for direct hit only)
     */
    ProjectileTower(const sf::Vector2f& pos = sf::Vector2f(0, 0),
                   float attackRange = 180.0f, int attackDamage = 35,
                   float attackSpeed = 0.8f, float projSpeed = 300.0f,
                   float splash = 0.0f);

    /**
     * @brief Destructor
     */
    ~ProjectileTower() override = default;

    /**
     * @brief Create and launch projectiles at targets
     * 
     * @param targets Vector of targets to fire at
     */
    void executeAction(const std::vector<Enemy*>& targets) override;
    
    /**
     * @brief Update the projectile tower's behavior
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
    
    /**
     * @brief Launch a projectile at the target
     * 
     * @param target The target enemy
     */
    void launchProjectile(Enemy* target);
    
    // Getters and setters
    float getProjectileSpeed() const { return projectileSpeed; }
    void setProjectileSpeed(float speed) { projectileSpeed = speed; }
    
    float getSplashRadius() const { return splashRadius; }
    void setSplashRadius(float radius) { splashRadius = radius; }
    
    float getAttackSpeed() const { return attackSpeed; }
    void setAttackSpeed(float speed) { attackSpeed = speed; }
};

#endif // PROJECTILE_TOWER_HPP
