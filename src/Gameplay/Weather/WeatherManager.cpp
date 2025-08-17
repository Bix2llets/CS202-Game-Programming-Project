/**
 * @file WeatherManager.cpp
 * @brief Implementation of the WeatherManager class.
 */

#include "Gameplay/Weather/WeatherManager.hpp"
#include "Gameplay/Weather/Weathers/SunnyWeather.hpp"
#include "Gameplay/Weather/Weathers/RainingWeather.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Scene/Level.hpp"
#include "EntityManager.hpp"
#include "Utility/logger.hpp"

WeatherManager::WeatherManager(Level& level) 
    : level(level), currentWeather(nullptr), currentWaveIndex(-1) {
    // Initialize all weather instances
    initializeWeatherInstances();
    
    // Start with sunny weather by default
    currentWeather = weatherInstances[WeatherType::Sunny].get();

    waveWeatherPattern = std::vector<WeatherType>(100, WeatherType::Raining);

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

void WeatherManager::nextWave() {
    changeWeatherForWave(currentWaveIndex + 1);
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
    
    // Switch to the appropriate weather instance
    auto it = weatherInstances.find(newWeatherType);
    if (it != weatherInstances.end()) {
        currentWeather = it->second.get();
    } else {
        currentWeather = weatherInstances[WeatherType::Sunny].get();
        Logger::warning("WeatherManager: Unknown weather type, using Sunny");
    }
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

inline void WeatherManager::applyWeatherToTower(Tower* tower) {
    currentWeather->applyToTower(tower);
}

inline void WeatherManager::applyWeatherToEnemy(Enemy* enemy) {
    currentWeather->applyToEnemy(enemy);
}

inline void WeatherManager::removeWeatherFromTower(Tower* tower) {
    currentWeather->removeFromTower(tower);
}

inline void WeatherManager::removeWeatherFromEnemy(Enemy* enemy) {
    currentWeather->removeFromEnemy(enemy);
}

void WeatherManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (currentWeather) {
        currentWeather->draw(target, states);
    }
}

void WeatherManager::initializeWeatherInstances() {
    // Create all weather instances
    weatherInstances[WeatherType::Sunny] = std::make_unique<SunnyWeather>(level);
    weatherInstances[WeatherType::Raining] = std::make_unique<RainingWeather>(level);
    
    // TODO: Add other weather types when implemented
    // weatherInstances[WeatherType::Thunderstorm] = std::make_unique<ThunderstormWeather>(level); // Temporary
    // weatherInstances[WeatherType::Foggy] = std::make_unique<FoggyWeather>(level); // Temporary
    
    Logger::info("WeatherManager: Initialized all weather instances");
}

void WeatherManager::clearCurrentWeatherEffects() {
    if (!currentWeather) {
        return;
    }
    
    EntityManager& entityManager = level.getEntityManager();
    
    for (Tower* tower : entityManager.getTowers()) {
        if (tower) removeWeatherFromTower(tower);
    }

    for (Enemy* enemy : entityManager.getEnemies()) {
        if (enemy && enemy->isAlive()) removeWeatherFromEnemy(enemy);
    }

    Logger::debug("WeatherManager: Cleared all current weather effects");
}

void WeatherManager::applyCurrentWeatherEffects() {
    if (!currentWeather) {
        return;
    }
    
    EntityManager& entityManager = level.getEntityManager();
    
    for (Tower* tower : entityManager.getTowers()) {
        if (tower) applyWeatherToTower(tower);
    }

    for (Enemy* enemy : entityManager.getEnemies()) {
        if (enemy && enemy->isAlive()) applyWeatherToEnemy(enemy);
    }
    
    Logger::debug("WeatherManager: Applied weather effects to all existing entities");
}
