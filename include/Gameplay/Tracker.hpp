#pragma once

/**
 * @file Tracker.hpp
 * @brief Declares the Tracker class for tracking gameplay statistics and events.
 *
 * The Tracker class is responsible for monitoring and recording various gameplay
 * events and statistics within a level, such as damage dealt, enemies defeated,
 * resources generated, and other gameplay-related metrics.
 */

#include <unordered_map>
#include <vector>
#include <memory>
#include "Gameplay/Currency.hpp"

// Forward declarations
class Level;
class Tower;
class Enemy;

/**
 * @class Tracker
 * @brief Tracks gameplay statistics and events for a specific level.
 *
 * Each level has a unique tracker that monitors various gameplay events
 * such as damage dealt by towers, damage taken by enemies, enemy deaths,
 * resource generation, and other gameplay metrics.
 */
class Tracker {
private:
    Level& level;  ///< Reference to the associated level
    
    // Tower-related tracking
    std::unordered_map<Tower*, float> towerDamageDealt;  ///< Damage dealt by each tower
    std::unordered_map<Tower*, int> towerKillCount;      ///< Number of enemies killed by each tower
    
    // Enemy-related tracking
    std::unordered_map<Enemy*, float> enemyDamageTaken;  ///< Damage taken by each enemy
    std::unordered_map<std::string, int> enemyDeathsByType;  ///< Deaths by enemy type
    
    // Level statistics
    int totalEnemiesKilled;        ///< Total number of enemies defeated
    float totalDamageDealt;        ///< Total damage dealt in the level
    Currency totalCurrencyGenerated;  ///< Total currency generated
    int totalProjectilesFired;     ///< Total projectiles fired
    
public:
    /**
     * @brief Constructs a tracker for the specified level.
     * @param levelRef Reference to the level this tracker monitors.
     */
    explicit Tracker(Level& levelRef);
    
    /**
     * @brief Destructor for the tracker.
     */
    ~Tracker() = default;
    
    /**
     * @brief Gets the associated level.
     * @return Reference to the level this tracker monitors.
     */
    Level& getLevel() const;
    
    /**
     * @brief Records damage dealt by a tower to an enemy.
     * @param tower Pointer to the tower dealing damage.
     * @param enemy Pointer to the enemy receiving damage.
     * @param damage Amount of damage dealt.
     */
    void recordDamage(Tower* tower, Enemy* enemy, float damage);
    
    /**
     * @brief Records the death of an enemy.
     * @param enemy Pointer to the enemy that died.
     * @param killerTower Pointer to the tower that killed the enemy (can be nullptr).
     */
    void recordEnemyDeath(Enemy* enemy, Tower* killerTower = nullptr);
    
    /**
     * @brief Records currency generation.
     * @param amount Amount of currency generated.
     */
    void recordCurrencyGeneration(const Currency& amount);
    
    /**
     * @brief Records a projectile being fired.
     * @param tower Pointer to the tower that fired the projectile.
     */
    void recordProjectileFired(Tower* tower);
    
    /**
     * @brief Gets the total damage dealt by a specific tower.
     * @param tower Pointer to the tower.
     * @return Total damage dealt by the tower.
     */
    float getTowerDamage(Tower* tower) const;
    
    /**
     * @brief Gets the total damage taken by a specific enemy.
     * @param enemy Pointer to the enemy.
     * @return Total damage taken by the enemy.
     */
    float getEnemyDamage(Enemy* enemy) const;
    
    /**
     * @brief Gets the total number of enemies killed.
     * @return Total number of enemies killed in this level.
     */
    int getTotalEnemiesKilled() const;
    
    /**
     * @brief Gets the total damage dealt in the level.
     * @return Total damage dealt by all towers.
     */
    float getTotalDamageDealt() const;
    
    /**
     * @brief Gets the total currency generated in the level.
     * @return Total currency generated.
     */
    Currency getTotalCurrencyGenerated() const;
    
    /**
     * @brief Resets all tracking data.
     */
    void reset();
    
private:
    /**
     * @brief Initializes tracking data structures.
     */
    void initialize();
};
