//
// AreaEffect.hpp
// A lingering ground effect that damages enemies while active
//
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_set>

#include "Entity/Entity.hpp"
#include "Entity/Damageable.hpp"
#include "Entity/Modules/Timer.hpp"
#include "Entity/Modules/EntityStat.hpp"
#include "Entity/Modules/SpriteAnimation.hpp"

class Enemy;
class Level;

/**
 * AreaEffect: a lingering ground hazard that periodically damages or apply effects to entities
 * inside a radius for a fixed duration (e.g., rumble after a bomb explodes).
 */
class AreaEffect : public Entity {
private:
    std::string id;         // Unique identifier for the area effect

    Level* levelRef;        // Cached Level pointer if scene is a Level
    float radius;           // Effect radius in world units
    DamageType type;        // Damage type
    
    Timer lifeTimer;        // Single-shot lifetime timer
    Timer tickTimer;        // Continuous tick timer

    int maxRepeats;         // Maximum number of repeats
    int currentRepeats;    // Current number of repeats

    bool active;            // Whether the effect is still active

    EntityStat stat;    // Entity statistics

    SpriteAnimation animation; // Animation for the area effect

    friend class AreaEffectFactory;
    
    void applyTickEffect();
    bool isCollidedWith(const sf::Vector2f position) const;

public:
    AreaEffect(Scene& scene);

    ~AreaEffect() override = default;

    // Entity overrides
    void update() override;
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
    void setPosition(const sf::Vector2f& pos) override;
    void setRotation(const sf::Angle& rot) override;

    // Lifetime/status
    bool isActive() const { return active; }
    float getRemainingLifetime() const { return lifeTimer.getRemainingTime(); }

    // Tuning
    void setUp();

    void setRadius(float r) { radius = r; }
    float getRadius() const { return radius; }
    void setDamageType(DamageType t) { type = t; }
    DamageType getDamageType() const { return type; }
    int getMaxRepeats() const { return maxRepeats; }
    void setMaxRepeats(int count) { maxRepeats = count; }
    int getCurrentRepeats() const { return currentRepeats; }
    void setCurrentRepeats(int count) { currentRepeats = count; }
    void increaseCurrentRepeats() { ++currentRepeats; }
    EntityStat& getStats() { return stat; }
    void setStats(const EntityStat& newStats) { stat = newStats; }

    // Visuals
    void updateSpriteAnimation();
    void updateSpriteSize();
    void loadSpriteAnimation(const nlohmann::json& animationPath);
};
