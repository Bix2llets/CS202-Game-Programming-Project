#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "EntitySystem.hpp"
#include <vector>
#include <memory>
#include <algorithm>

/**
 * @brief Manager class for handling collections of entities
 * 
 * This class provides centralized management for all game entities,
 * including update loops, rendering, and cleanup operations.
 */
class EntityManager {
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Projectile>> projectiles;

public:
    /**
     * @brief Update all entities
     * 
     * @param deltaTime Time elapsed since last update
     */
    void updateAll(float deltaTime);

    /**
     * @brief Render all entities
     * 
     * @param target Render target to draw on
     */
    void renderAll(sf::RenderTarget& target);

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
     * @brief Get all enemies (for tower targeting)
     * 
     * @return std::vector<Enemy*> Vector of enemy pointers
     */
    std::vector<Enemy*> getEnemies();

    /**
     * @brief Get all towers
     * 
     * @return std::vector<Tower*> Vector of tower pointers
     */
    std::vector<Tower*> getTowers();

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
};

#endif // ENTITY_MANAGER_HPP
