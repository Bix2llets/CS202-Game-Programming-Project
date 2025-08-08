
#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "Core/KeyboardObserver.hpp"
#include "Core/MouseObserver.hpp"

#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/Tower/Projectile/Projectile.hpp"
#include "Entity/AreaEffect/AreaEffect.hpp"


class Level;
/**
 * @brief Manager class for handling collections of entities
 *
 * This class provides centralized management for all game entities,
 * including update loops, rendering, and cleanup operations.
 */
class EntityManager : public MouseObserver, public KeyboardObserver {
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    std::vector<std::unique_ptr<AreaEffect>> areaEffects;

    Level& level;

public:
    EntityManager(Level& parentLevel) : level{parentLevel} {}
    /**
     * @brief Update all entities
     *
     * @param deltaTime Time elapsed since last update
     */
    void update();

    /**
     * @brief Render all entities
     *
     * @param target Render target to draw on
     */
    void render(sf::RenderStates state) const;

    /**
     * @brief Clean up dead entities
     */
    void cleanup();

    /**
     * @brief Add a tower to the game
     *
     * @param tower Tower to add
     */
    void addTower(std::unique_ptr<Tower> tower);

    /**
     * @brief Add an enemy to the game
     *
     * @param enemy Enemy to add
     */
    void addEnemy(std::unique_ptr<Enemy> enemy);

    /**
     * @brief Add a projectile to the game
     *
     * @param projectile Projectile to add
     */
    void addProjectile(std::unique_ptr<Projectile> projectile);

    /**
     * @brief Add an area effect to the game
     *
     * @param effect AreaEffect to add
     */
    void addAreaEffect(std::unique_ptr<AreaEffect> effect);

    /**
     * @brief Get all enemies (for tower targeting)
     *
     * @return std::vector<Enemy*> Vector of enemy pointers
     */
    std::vector<Enemy *> getEnemies();

    /**
     * @brief Get all towers
     *
     * @return std::vector<Tower*> Vector of tower pointers
     */
    std::vector<Tower *> getTowers();

    /**
     * @brief Get all active area effects
     */
    std::vector<AreaEffect*> getAreaEffects();

    /**
     * @brief Clear all entities
     */
    void clear();

    /**
     * @brief Get total entity count
     *
     * @return size_t Total number of entities
     */
    size_t getTotalEntityCount() const;

    bool onMouseEvent(Mouse button, UserEvent event,
                      const sf::Vector2f &worldPosition,
                      const sf::Vector2f &windowPosition) override;

    bool onKeyEvent(Key key, UserEvent event, const sf::Vector2f &worldPosition,
                    const sf::Vector2f &windowPosition) override;
    bool onScrollEvent(float delta, const sf::Vector2f &worldPosition,
                       const sf::Vector2f &windowPosition) override;


   void removeTower(Tower* tower);
};
