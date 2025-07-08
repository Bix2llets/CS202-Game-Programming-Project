/**
 * @file Projectile.hpp
 * @brief Declares the Projectile class for projectiles fired by towers.
 *
 * Projectiles can target entities or locations, move, and detect collisions.
 */
#pragma once
#include <unordered_map>

#include "Entity/Damageable.hpp"
#include "Entity/Entity.hpp"
#include "Entity/Health.hpp"
// Forward declarations
class Enemy;
class Scene;

/**
 * @enum ProjectileType
 * @brief Enumeration for projectile target types.
 */
enum class ProjectileType {
    TargetEntity,   ///< Targets and tracks an entity
    TargetLocation  ///< Targets a fixed location
};

/**
 * @class Projectile
 * @brief Projectile entity fired by towers to attack enemies.
 *
 * Handles movement, collision detection, and damage application.
 */
class Projectile : public Entity {
private:
    Health hitRemaining; ///< Remaining hits or health for the projectile
    int damage; ///< Damage dealt by the projectile
    float speed; ///< Movement speed
    bool hasHit; ///< Whether the projectile has hit its target
    sf::Vector2f velocity; ///< Current velocity vector
    Enemy* targetEntity; ///< Target entity (if any)
    sf::Vector2f targetLocation; ///< Target location (if any)
    ProjectileType projectileType; ///< Type of projectile (entity or location)
    float collisionDistance; ///< Collision distance threshold

    /**
     * @brief Construct a new Projectile object (private, for factory use).
     * @param scene Reference to the scene this projectile belongs to.
     */
    Projectile(Scene& scene, const sf::Texture &texture);
    friend class ProjectileFactory;

public:
    /**
     * @brief Destructor.
     */
    ~Projectile() override = default;

    /**
     * @brief Update the projectile's movement and collision.
     * Handles movement, collision checks, and state updates.
     */
    void update() override;

    /**
     * @brief Render the projectile.
     * @param target Render target to draw on.
     * @param state The render state of object.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

    /**
     * @brief Check if the projectile has collided with its target.
     * @return bool True if collision occurred, false otherwise.
     */
    bool isCollided();

    /**
     * @brief Check collision with a specific position.
     * @param pos Position to check collision with.
     * @return bool True if within collision distance.
     */
    bool isCollided(const sf::Vector2f& pos) const;

    /**
     * @brief Get the target entity (if any).
     * @return Pointer to the target entity.
     */
    Enemy* getTargetEntity() const { return targetEntity; }

    /**
     * @brief Get the target location (if any).
     * @return Target location vector.
     */
    sf::Vector2f getTargetLocation() const { return targetLocation; }

    /**
     * @brief Get the projectile's movement speed.
     * @return Speed value.
     */
    float getSpeed() const { return speed; }

    /**
     * @brief Set the projectile's movement speed.
     * @param newSpeed New speed value.
     */
    void setSpeed(float newSpeed) { speed = newSpeed; }

    /**
     * @brief Get the projectile type.
     * @return ProjectileType value.
     */
    ProjectileType getProjectileType() const { return projectileType; }

    /**
     * @brief Get the collision distance threshold.
     * @return float Collision distance.
     */
    float getCollisionDistance() { return collisionDistance; }

    /**
     * @brief Check if the projectile is still alive.
     * @return bool True if alive, false otherwise.
     */
    inline bool isAlive() {
        return static_cast<bool>(hitRemaining.getHealth() != 0);
    }

protected:
    /**
     * @brief Called when the projectile is destroyed or expires.
     */
    void onDeath();

private:
    /**
     * @brief Move the projectile towards its target.
     */
    void move();

    /**
     * @brief Handle collision with any object.
     */
    void onCollision();

    /**
     * @brief Handle collision with a specific enemy.
     * @param enemy Reference to the enemy collided with.
     */
    void onCollision(Enemy& enemy);
};
