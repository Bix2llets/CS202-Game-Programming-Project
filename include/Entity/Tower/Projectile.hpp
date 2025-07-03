#pragma once
#include "Entity/Entity.hpp"
#include "Entity/Damageable.hpp"
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
class Projectile : public Entity, public Damageable {
   private:
    int damage;
    float speed;
    bool hasHit;
    sf::Vector2f velocity;
    Enemy* targetEntity;
    sf::Vector2f targetLocation;
    ProjectileType projectileType;
    static constexpr float COLLISION_DISTANCE =
        10.0f;  // Fixed collision distance

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
     */
    void update() override;

    /**
     * @brief Render the projectile
     *
     * @param target Render target to draw on
     * @param state The render state of object
     */
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

    /**
     * @brief Check if the projectile has collided with its target
     *
     * @return bool True if collision occurred, false otherwise
     */
    bool isCollided();

    /**
     * @brief Check collision with a specific position
     *
     * @param pos Position to check collision with
     * @return bool True if within collision distance
     */
    bool isCollidedWith(const sf::Vector2f& pos) const;

    // Getters and setters

    inline int getDamage() const { return damage; }
    void setDamage(int newDamage) { damage = newDamage; }

    Enemy* getTargetEntity() const { return targetEntity; }
    sf::Vector2f getTargetLocation() const { return targetLocation; }

    float getSpeed() const { return speed; }
    void setSpeed(float newSpeed) { speed = newSpeed; }

    bool isHit() const { return hasHit; }

    ProjectileType getProjectileType() const { return projectileType; }

    /**
     * @brief Get the collision distance threshold
     *
     * @return float Collision distance
     */
    static float getCollisionDistance() { return COLLISION_DISTANCE; }
    protected: 
    void onDeath();
   private:
    /**
     * @brief Move the projectile towards its target
     */
    void move();
};
