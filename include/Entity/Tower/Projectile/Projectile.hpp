/**
 * @file Projectile.hpp
 * @brief Declares the Projectile class for projectiles fired by towers.
 *
 * Projectiles can target entities or locations, move, and detect collisions.
 */
#pragma once

#include "Entity/Entity.hpp"
#include "Entity/AreaEffect/AreaEffect.hpp"
#include "Entity/Modules/EntityStat.hpp"
#include <memory>

// Forward declarations
class Enemy;
class Tower;
class Scene;
class Level;
class FlightMode;

#include "Entity/Modules/SpriteAnimation.hpp"

/**
 * @enum ProjectileType
 * @brief Enumeration for projectile target types.
 */
enum class ProjectileTargetType {
    Trajectory,    ///< Moves along a trajectory
    TargetEntity,   ///< Targets and tracks an entity
    TargetLocation  ///< Targets a fixed locatio
};

/**
 * @class Projectile
 * @brief Projectile entity fired by towers to attack enemies.
 *
 * Handles movement, collision detection, and damage application.
 */
class Projectile : public Entity {
private:
    SpriteAnimation animation; ///< Animation for the projectile sprite

    std::string id; ///< Unique identifier for the projectile

    ProjectileTargetType type; ///< Type of projectile (target entity or location)

    bool rotateToTarget; ///< Whether to rotate towards target
    int pierceCount; ///< Number of enemies this projectile can pierce through
    int currentPierceCount; ///< Current number of pierced enemies

    float collisionDistance; ///< Collision distance threshold
    float speed; ///< Movement speed
    sf::Vector2f velocity; ///< Current velocity vector
    
    bool flying; ///< Whether the projectile has hit its target
    std::vector<Enemy*> hitEnemies; ///< Enemies hit by this projectile
    
    Tower* source; ///< Tower that fired this projectile
    uint16_t sourceId; ///< ID of the source tower
    EntityStat stats; ///< Stats of the source tower

    Enemy* targetEntity; ///< Target entity (if any)
    sf::Vector2f targetLocation; ///< Target location

    FlightMode* flightMode; ///< Flying behavior mode for the projectile

    std::vector<std::unique_ptr<AreaEffect>> onHitAreaEffect; ///< Area effect when the projectile hits

    /**
     * @brief Construct a new Projectile object (private, for factory use).
     * @param scene Reference to the scene this projectile belongs to.
     */
    friend class ProjectileFactory;
    friend class FlightMode; ///< Allow FlightMode to access private members
    friend class LinearFlightMode; ///< Allow LinearFlightMode to access private members
    
    inline bool hasHitEnemy(Enemy* enemy) const;
    inline bool isCollidedWith(sf::Vector2f position) const;

public:
    Projectile(Scene& scene, const std::string id);

    Projectile(const Projectile& other); ///< Copy constructor (deep copy).
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
     * @brief Update the sprite animation for the projectile.
     */
    void updateSpriteAnimation();

    /**
     * @brief Load the projectile's sprite animation from a JSON file.
     * @param spriteAnimationPath Path to the JSON file containing animation data.
     */
    void loadSpriteAnimation(const nlohmann::json& spriteAnimationPath);

    /**
     * @brief Render the projectile.
     * @param target Render target to draw on.
     * @param state The render state of object.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

    ProjectileTargetType getType() const { return type; }

    int getPierceCount() const { return pierceCount; }
    int getCurrentPierceCount() const { return currentPierceCount; }

    float getCollisionDistance() const { return collisionDistance; }
    float getSpeed() const { return speed; }
    sf::Vector2f getVelocity() const { return velocity; }
    bool isFlying() const { return flying; }

    const std::vector<Enemy*>& getHitEnemies() const { return hitEnemies; }

    Tower* getSource() const { return source; }

    Enemy* getTargetEntity() const { return targetEntity; }
    sf::Vector2f getTargetLocation() const { return targetLocation; }

    FlightMode* getFlightMode() const { return flightMode; }

    // Override Entity pure virtual methods
    void setPosition(const sf::Vector2f& pos) override;
    void setRotation(const sf::Angle& rot) override;
    
    // Setters
    void bindToTower(Tower* tower);
    void stopFlying();
    void setUpFlightMode();
    void setTarget(Enemy* enemy);
    void setFlightMode(FlightMode* mode) { flightMode = mode; }
    void setTargetType(ProjectileTargetType targetType) { type = targetType; }
    void setPierceCount(int count) { pierceCount = count; }
    void setCurrentPierceCount(int count) { currentPierceCount = count; }
    void addOnHitAreaEffect(std::unique_ptr<AreaEffect> effect) { onHitAreaEffect.push_back(std::move(effect)); }
    void increaseCurrentPierceCount() { ++currentPierceCount; }
    void setCollisionDistance(float distance) { collisionDistance = distance; }
    void setRotateToTarget(bool rotate) { rotateToTarget = rotate; }
};
