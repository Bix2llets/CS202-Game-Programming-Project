/**
 * @file Tracker.cpp
 * @brief Implementation of the Tracker class for tracking gameplay statistics.
 */

#include "Gameplay/Tracker.hpp"
#include "Scene/Level.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/Enemy/Enemy.hpp"

Tracker::Tracker(Level& levelRef) 
    : level(levelRef), 
      totalEnemiesKilled(0),
      totalDamageDealt(0.0f),
      totalCurrencyGenerated(Currency(0, 0)),
      totalProjectilesFired(0) {
    initialize();
}

Level& Tracker::getLevel() const {
    return level;
}

void Tracker::recordDamage(Tower* tower, Enemy* enemy, float damage) {
    if (tower && enemy && damage > 0.0f) {
        // Record damage dealt by tower
        towerDamageDealt[tower] += damage;
        
        // Record damage taken by enemy
        enemyDamageTaken[enemy] += damage;
        
        // Update total damage
        totalDamageDealt += damage;
    }
}

void Tracker::recordEnemyDeath(Enemy* enemy, Tower* killerTower) {
    if (enemy) {
        // Increment total enemies killed
        ++totalEnemiesKilled;
        
        // Record kill for the tower if specified
        if (killerTower) {
            towerKillCount[killerTower]++;
        }
        
        // TODO: Record death by enemy type when enemy type system is implemented
        // enemyDeathsByType[enemy->getType()]++;
    }
}

void Tracker::recordCurrencyGeneration(const Currency& amount) {
    totalCurrencyGenerated += amount;
}

void Tracker::recordProjectileFired(Tower* tower) {
    if (tower) {
        ++totalProjectilesFired;
    }
}

float Tracker::getTowerDamage(Tower* tower) const {
    auto it = towerDamageDealt.find(tower);
    return (it != towerDamageDealt.end()) ? it->second : 0.0f;
}

float Tracker::getEnemyDamage(Enemy* enemy) const {
    auto it = enemyDamageTaken.find(enemy);
    return (it != enemyDamageTaken.end()) ? it->second : 0.0f;
}

int Tracker::getTotalEnemiesKilled() const {
    return totalEnemiesKilled;
}

float Tracker::getTotalDamageDealt() const {
    return totalDamageDealt;
}

Currency Tracker::getTotalCurrencyGenerated() const {
    return totalCurrencyGenerated;
}

void Tracker::reset() {
    // Clear all tracking data
    towerDamageDealt.clear();
    towerKillCount.clear();
    enemyDamageTaken.clear();
    enemyDeathsByType.clear();
    
    // Reset counters
    totalEnemiesKilled = 0;
    totalDamageDealt = 0.0f;
    totalCurrencyGenerated = Currency(0, 0);
    totalProjectilesFired = 0;
}

void Tracker::initialize() {
    // Initialize data structures
    towerDamageDealt.clear();
    towerKillCount.clear();
    enemyDamageTaken.clear();
    enemyDeathsByType.clear();
}
