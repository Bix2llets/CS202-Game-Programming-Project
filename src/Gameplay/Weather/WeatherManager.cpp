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
#include <string>
#include "Utility/logger.hpp"

WeatherManager::WeatherManager(Level& level) 
    : level(level), currentWeather(nullptr), previousWeather(nullptr), currentWaveIndex(-1) {}

void WeatherManager::loadJSON(const nlohmann::json& jsonFile) {
    auto weatherConfig = jsonFile["wave"];

    for (auto it = weatherConfig.begin(); it != weatherConfig.end(); ++it) {
        if(!(*it).contains("weather")) {
            waveWeatherPattern.push_back(WeatherType::Sunny); // Default to Sunny if no weather specified
            Logger::warning("WeatherManager: No weather specified, defaulting to Sunny");
            continue;
        }
        std::string wt = (*it)["weather"].get<std::string>();
        
        if (wt == "sunny" || wt == "default") {
            waveWeatherPattern.push_back(WeatherType::Sunny);
            Logger::critical("WeatherManager: Sunny");
        } else if (wt == "raining" || wt == "rainy" || wt == "rain") {
            waveWeatherPattern.push_back(WeatherType::Raining);
            Logger::critical("WeatherManager: Raining");
        } else if (wt == "thunderstorm" || wt == "thunder" || wt == "storm") {
            waveWeatherPattern.push_back(WeatherType::Thunderstorm);
            Logger::critical("WeatherManager: Thunderstorm");
        } else if (wt == "foggy" || wt == "fog") {
            waveWeatherPattern.push_back(WeatherType::Foggy);
            Logger::critical("WeatherManager: Foggy");
        } else {
            waveWeatherPattern.push_back(WeatherType::Sunny);
            Logger::warning("WeatherManager: Unknown weather type '" + wt + "', defaulting to Sunny");
        }
    }

    setUp();
}

void WeatherManager::setUp() {
    initializeWeatherInstances();
    
    // Set initial weather - use first pattern if available, otherwise default to Sunny
    WeatherType initialWeather = WeatherType::Sunny;
    if (!waveWeatherPattern.empty()) {
        initialWeather = waveWeatherPattern[0];
    }
    
    auto it = weatherInstances.find(initialWeather);
    if (it != weatherInstances.end()) {
        currentWeather = it->second.get();
    } else {
        currentWeather = weatherInstances[WeatherType::Sunny].get();
        Logger::warning("WeatherManager: Initial weather type not found, using Sunny");
    }
}

WeatherManager::~WeatherManager() {
    // Clear all weather effects before destruction
    clearCurrentWeatherEffects();
}

void WeatherManager::update() {
    if (currentWeather) {
        currentWeather->update();
    }
    
    // Only update previous weather if it's different from current weather and has opacity > 0
    if (previousWeather && previousWeather != currentWeather && previousWeather->getOverlayOpacity() > 0.0f) {
        previousWeather->update();
        
        // Reset previousWeather to nullptr when it's fully faded out
        if (previousWeather->getOverlayOpacity() <= 0.0f) {
            previousWeather = nullptr;
        }
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


    // Lambda function: WeatherType -> string
    auto weatherTypeToString = [](WeatherType type) {
        switch (type) {
            case WeatherType::Sunny: return "Sunny";
            case WeatherType::Raining: return "Raining";
            case WeatherType::Thunderstorm: return "Thunderstorm";
            case WeatherType::Foggy: return "Foggy";
            default: return "Unknown";
        }
    };

    WeatherType currentWeatherType = getCurrentWeatherType();
    Logger::critical(std::string("WeatherManager: Attempting to change weather from ") + weatherTypeToString(currentWeatherType) + " to " + weatherTypeToString(newWeatherType));
    if(currentWeatherType == newWeatherType) {
        previousWeather = nullptr;
        currentWaveIndex = waveIndex;
        return; // No need to change weather
    }


    // Clear current weather effects
    clearCurrentWeatherEffects();
    
    // Store previous weather before changing current weather (only if it's different)
    Weather* newWeatherInstance = nullptr;
    
    // Switch to the appropriate weather instance
    auto it = weatherInstances.find(newWeatherType);
    if (it != weatherInstances.end()) {
        newWeatherInstance = it->second.get();
    } else {
        newWeatherInstance = weatherInstances[WeatherType::Sunny].get();
        Logger::warning("WeatherManager: Unknown weather type, using Sunny");
    }
    
    // Only set previous weather if we're actually changing to a different weather instance
    if (newWeatherInstance != currentWeather) {
        previousWeather = currentWeather;
        currentWeather = newWeatherInstance;
    } else {
        // If it's the same instance, don't change anything
        Logger::debug("WeatherManager: Weather instance is the same, no transition needed");
        return;
    }

    // Safely set properties on current weather
    if (currentWeather) {
        currentWeather->setOverlayOpacity(0.0f);
        currentWeather->setOverlayOpacityChange(0.01f);
    }
    
    // Safely set properties on previous weather
    if (previousWeather) {
        previousWeather->setOverlayOpacityChange(-0.01f);
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

void WeatherManager::applyWeatherToTower(Tower* tower) {
    if (currentWeather && tower) {
        currentWeather->applyToTower(tower);
    }
}

void WeatherManager::applyWeatherToEnemy(Enemy* enemy) {
    if (currentWeather && enemy) {
        currentWeather->applyToEnemy(enemy);
    }
}

void WeatherManager::removeWeatherFromTower(Tower* tower) {
    if (currentWeather && tower) {
        currentWeather->removeFromTower(tower);
    }
}

void WeatherManager::removeWeatherFromEnemy(Enemy* enemy) {
    if (currentWeather && enemy) {
        currentWeather->removeFromEnemy(enemy);
    }
}

void WeatherManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Draw previous weather first (if it exists and is different from current)
    if (previousWeather && previousWeather != currentWeather && previousWeather->getOverlayOpacity() > 0.0f) {
        previousWeather->draw(target, states);
    }
    
    // Draw current weather on top
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
