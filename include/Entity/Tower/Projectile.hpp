#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Entity/Entity.hpp"

// Forward declarations
class Enemy;

/**
 * @brief Enumeration for projectile target types
 */
enum class ProjectileType {
    TargetEntity,   // Targets and tracks an entity
    TargetLocation  // Targets a fixed location
};

/**
 * @brief Projectile entity fired by towers to attack enemies
 */
class Projectile : public Entity {
private:
    sf::Vector2f velocity;
    int damage;
    Enemy* targetEntity;
    sf::Vector2f targetLocation;
    float speed;
    bool hasHit;
    ProjectileType projectileType;
    static constexpr float COLLISION_DISTANCE = 10.0f; // Fixed collision distance

public:
    /**
     * @brief Construct a new Entity-targeting Projectile
     * 
     * @param pos Initial position
     * @param targetEnemy Target enemy to hit
     * @param projectileDamage Damage to deal on hit
     * @param projectileSpeed Movement speed
     */
    Projectile(const sf::Vector2f& pos, Enemy* targetEnemy, 
               int projectileDamage = 25, float projectileSpeed = 300.0f);

    /**
     * @brief Construct a new Location-targeting Projectile
     * 
     * @param pos Initial position
     * @param targetPos Target location to hit
     * @param projectileDamage Damage to deal on hit
     * @param projectileSpeed Movement speed
     */
    Projectile(const sf::Vector2f& pos, const sf::Vector2f& targetPos, 
               int projectileDamage = 25, float projectileSpeed = 300.0f);

    /**
     * @brief Destructor
     */
    ~Projectile() override = default;

    /**
     * @brief Update the projectile's movement and collision
     * 
     * @param deltaTime Time elapsed since last update
     */
    void update(float deltaTime) override;

    /**
     * @brief Render the projectile
     * 
     * @param target Render target to draw on
     */
    void render(sf::RenderTarget& target) override;

    /**
     * @brief Move the projectile towards its target
     * 
     * @param deltaTime Time elapsed since last update
     */
    void move(float deltaTime);

    /**
     * @brief Check if the projectile has collided with its target
     * 
     * @return bool True if collision occurred, false otherwise
     */
    bool checkCollision();

    /**
     * @brief Check collision with a specific position
     * 
     * @param pos Position to check collision with
     * @return bool True if within collision distance
     */
    bool checkCollisionWithPosition(const sf::Vector2f& pos) const;

    // Getters and setters
    sf::Vector2f getVelocity() const { return velocity; }
    void setVelocity(const sf::Vector2f& vel) { velocity = vel; }

    int getDamage() const { return damage; }
    void setDamage(int newDamage) { damage = newDamage; }

    Enemy* getTargetEntity() const { return targetEntity; }
    sf::Vector2f getTargetLocation() const { return targetLocation; }

    float getSpeed() const { return speed; }
    void setSpeed(float newSpeed) { speed = newSpeed; }

    bool getHasHit() const { return hasHit; }
    
    ProjectileType getProjectileType() const { return projectileType; }

    /**
     * @brief Get the collision distance threshold
     * 
     * @return float Collision distance
     */
    static float getCollisionDistance() { return COLLISION_DISTANCE; }
};

#endif // PROJECTILE_HPP
