

/**
 * @file Tower.hpp
 * @brief Declares the Tower base class for all tower entities in the game.
 *
 * The Tower class provides the common interface and properties for all tower types,
 * including cost, cooldown management, and upgrade logic. Specific tower types should
 * inherit from this class and implement their own behavior and rendering.
 */
#pragma once

#include <memory>
#include "Entity/Cooldown.hpp"
#include "Entity/Entity.hpp"

class Level;
class TowerStat;
class Scene;

/**
 * @class Tower
 * @brief Abstract base class for all tower entities.
 *
 * Towers are defensive structures that can be placed in the game world. They have a cost,
 * can be upgraded, and have cooldowns for their actions. Derived classes should implement
 * specific attack or support behaviors.
 */
class Tower : public Entity {
protected:
    Cooldown timer;                  ///< Cooldown timer for tower actions
    int cost;                        ///< Cost to build the tower
    std::unique_ptr<TowerStat> stats;///< Pointer to tower statistics/attributes

public:
    /**
     * @brief Construct a new Tower object
     *
     * @param pos Position to place the tower
     * @param angle Initial rotation angle
     * @param buildCost Cost to build the tower
     * @param scene Reference to the game scene
     */
    Tower(const sf::Vector2f& pos = sf::Vector2f(0, 0),
          const sf::Angle& angle = sf::radians(0.f), int buildCost = 100,
          const Scene& scene)
        : Entity(scene, position, angle), cost(cost) {}

    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    virtual ~Tower() override = default;

    /**
     * @brief Update the tower's behavior (pure virtual).
     */
    virtual void update() = 0;

    /**
     * @brief Render the tower (pure virtual).
     * @param target Render target to draw on
     * @param state Current render states
     */
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates state) const override = 0;

    /**
     * @brief Update both base sprite and upper layer sprite positions (pure virtual).
     * @param pos New position
     */
    virtual void setPosition(const sf::Vector2f& pos) = 0;

    /**
     * @brief Set the tower's upgrade level.
     * @param level New level
     */
    virtual void setLevel(Level level);

    /**
     * @brief Upgrade the tower along a specific path.
     * @param pathNumber The upgrade path number
     */
    virtual void upgradePath(int pathNumber);

    /**
     * @brief Check if the tower can be upgraded along a specific path.
     * @param pathNumber The upgrade path number
     * @return bool True if upgrade is possible, false otherwise
     */
    bool canUpgrade(int pathNumber);

    /**
     * @brief Called at the start of a round (pure virtual).
     */
    virtual void onRoundStart() = 0;

    /**
     * @brief Called at the end of a round (pure virtual).
     */
    virtual void onRoundEnd() = 0;
};
