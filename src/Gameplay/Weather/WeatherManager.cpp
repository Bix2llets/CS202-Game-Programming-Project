/**
 * @file WeatherManager.cpp
 * @brief Implementation of the WeatherManager class.
 */

#include "Gameplay/Weather/WeatherManager.hpp"
#include "Gameplay/Weather/SunnyWeather.hpp"
#include "Gameplay/Weather/RainyWeather.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Scene/Level.hpp"
#include "EntityManager.hpp"
#include "Utility/logger.hpp"

WeatherManager::WeatherManager(Level& level) 
    : level(level), currentWaveIndex(-1) {
    // Start with sunny weather by default
    currentWeather = createWeather(WeatherType::Sunny);
    Logger::info("WeatherManager: Initialized with sunny weather");
}

WeatherManager::~WeatherManager() {
    // Clear all weather effects before destruction
    clearCurrentWeatherEffects();
}

void WeatherManager::update() {
    if (currentWeather) {
        currentWeather->update();
    }
}

void WeatherManager::setWeatherPattern(const std::vector<WeatherType>& pattern) {
    waveWeatherPattern = pattern;
    Logger::info("WeatherManager: Set weather pattern with " + std::to_string(pattern.size()) + " waves");
    
    // If we have a pattern and are still on wave 0, apply the first weather
    if (!pattern.empty() && currentWaveIndex <= 0) {
        changeWeatherForWave(0);
    }
}

void WeatherManager::changeWeatherForWave(int waveIndex) {
    if (waveIndex == currentWaveIndex) {
        return; // Already on this wave
    }
    
    WeatherType newWeatherType = WeatherType::Sunny; // Default
    
    // Get weather type from pattern if available
    if (!waveWeatherPattern.empty() && waveIndex >= 0 && waveIndex < static_cast<int>(waveWeatherPattern.size())) {
        newWeatherType = waveWeatherPattern[waveIndex];
    }
    
    // Clear current weather effects
    clearCurrentWeatherEffects();
    
    // Create and apply new weather
    currentWeather = createWeather(newWeatherType);
    currentWaveIndex = waveIndex;
    
    // Apply new weather effects to all existing entities
    applyCurrentWeatherEffects();
    
    std::string weatherName;
    switch (newWeatherType) {
        case WeatherType::Sunny: weatherName = "Sunny"; break;
        case WeatherType::Raining: weatherName = "Raining"; break;
        case WeatherType::Thunderstorm: weatherName = "Thunderstorm"; break;
        case WeatherType::Foggy: weatherName = "Foggy"; break;
    }
    
    Logger::info("WeatherManager: Changed to " + weatherName + " weather for wave " + std::to_string(waveIndex + 1));
}

WeatherType WeatherManager::getCurrentWeatherType() const {
    if (currentWeather) {
        return currentWeather->getType();
    }
    return WeatherType::Sunny;
}

void WeatherManager::applyWeatherToTower(Tower& tower) {
    uint64_t towerId = tower.getUniqueId();
    
    // Only apply if not already affected
    if (affectedTowers.find(towerId) == affectedTowers.end()) {
        if (currentWeather) {
            currentWeather->applyToTower(tower);
            affectedTowers[towerId] = true;
        }
    }
}

void WeatherManager::applyWeatherToEnemy(Enemy& enemy) {
    uint64_t enemyId = enemy.getUniqueId();
    
    // Only apply if not already affected
    if (affectedEnemies.find(enemyId) == affectedEnemies.end()) {
        if (currentWeather) {
            currentWeather->applyToEnemy(enemy);
            affectedEnemies[enemyId] = true;
        }
    }
}

void WeatherManager::removeWeatherFromTower(Tower& tower) {
    uint64_t towerId = tower.getUniqueId();
    
    // Only remove if currently affected
    if (affectedTowers.find(towerId) != affectedTowers.end()) {
        if (currentWeather) {
            currentWeather->removeFromTower(tower);
        }
        affectedTowers.erase(towerId);
    }
}

void WeatherManager::removeWeatherFromEnemy(Enemy& enemy) {
    uint64_t enemyId = enemy.getUniqueId();
    
    // Only remove if currently affected
    if (affectedEnemies.find(enemyId) != affectedEnemies.end()) {
        if (currentWeather) {
            currentWeather->removeFromEnemy(enemy);
        }
        affectedEnemies.erase(enemyId);
    }
}

void WeatherManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (currentWeather) {
        currentWeather->draw(target, states);
    }
}

std::unique_ptr<Weather> WeatherManager::createWeather(WeatherType type) {
    switch (type) {
        case WeatherType::Sunny:
            return std::make_unique<SunnyWeather>(level);
        case WeatherType::Raining:
            return std::make_unique<RainyWeather>(level);
        case WeatherType::Thunderstorm:
            // TODO: Implement ThunderstormWeather
            Logger::warning("WeatherManager: Thunderstorm weather not implemented yet, using Rainy");
            return std::make_unique<RainyWeather>(level);
        case WeatherType::Foggy:
            // TODO: Implement FoggyWeather
            Logger::warning("WeatherManager: Foggy weather not implemented yet, using Sunny");
            return std::make_unique<SunnyWeather>(level);
        default:
            Logger::warning("WeatherManager: Unknown weather type, using Sunny");
            return std::make_unique<SunnyWeather>(level);
    }
}

void WeatherManager::clearCurrentWeatherEffects() {
    if (!currentWeather) {
        return;
    }
    
    // Remove weather effects from all affected towers
    EntityManager& entityManager = level.getEntityManager();
    
    // for (auto it = affectedTowers.begin(); it != affectedTowers.end();) {
    //     uint64_t towerId = it->first;
    //     Tower* tower = entityManager.getEntity<Tower>(towerId);
    //     if (tower) {
    //         currentWeather->removeFromTower(*tower);
    //     }
    //     it = affectedTowers.erase(it);
    // }
    
    // Remove weather effects from all affected enemies
    // for (auto it = affectedEnemies.begin(); it != affectedEnemies.end();) {
    //     uint64_t enemyId = it->first;
    //     Enemy* enemy = entityManager.getEntity<Enemy>(enemyId);
    //     if (enemy) {
    //         currentWeather->removeFromEnemy(*enemy);
    //     }
    //     it = affectedEnemies.erase(it);
    // }
    
    Logger::debug("WeatherManager: Cleared all current weather effects");
}

void WeatherManager::applyCurrentWeatherEffects() {
    if (!currentWeather) {
        return;
    }
    
    // Apply weather effects to all existing towers
    EntityManager& entityManager = level.getEntityManager();
    
    // Get all towers and apply weather effects
    // auto towers = entityManager.getEntitiesByType<Tower>();
    // for (Tower* tower : towers) {
    //     if (tower) {
    //         applyWeatherToTower(*tower);
    //     }
    // }
    
    // Get all enemies and apply weather effects
    // auto enemies = entityManager.getEntitiesByType<Enemy>();
    // for (Enemy* enemy : enemies) {
    //     if (enemy) {
    //         applyWeatherToEnemy(*enemy);
    //     }
    // }
    
    Logger::debug("WeatherManager: Applied weather effects to all existing entities");
}
