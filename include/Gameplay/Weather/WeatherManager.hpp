/**
 * @file WeatherManager.hpp
 * @brief Declares the WeatherManager class for managing weather effects.
 */
#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include "Weather.hpp"

// Forward declarations
class Level;
class Tower;
class Enemy;

/**
 * @class WeatherManager
 * @brief Manages weather effects and their application to game entities.
 */
class WeatherManager {
private:
    Level& level;
    Weather* currentWeather;
    Weather* previousWeather;
    std::vector<WeatherType> waveWeatherPattern;
    int currentWaveIndex;
    
    // Pre-created weather instances
    std::unordered_map<WeatherType, std::unique_ptr<Weather>> weatherInstances;
    
public:
    /**
     * @brief Construct a new WeatherManager.
     * @param level Reference to the game level.
     */
    WeatherManager(Level& level);

    /**
     * @brief Destructor.
     */
    ~WeatherManager();

    /**
    * @brief Load weather data from JSON file.
    * @param jsonFile JSON file containing weather data.
    */
    void loadJSON(const nlohmann::json& jsonFile);

    /**
     * @brief Set up weather manager.
     */
    void setUp();

    /**
     * @brief Update weather effects.
     */
    void update();


    /**
     * @brief Set weather pattern for all waves.
     * @param pattern Vector of weather types for each wave.
     */
    void setWeatherPattern(const std::vector<WeatherType>& pattern);

    /**
     * @brief Move to the next wave.
     */
    void nextWave();

    /**
     * @brief Change weather for a specific wave.
     * @param waveIndex Index of the wave (0-based).
     */
    void changeWeatherForWave(int waveIndex);

    /**
     * @brief Get current weather.
     * @return const Weather* Pointer to current weather, or nullptr if none.
     */
    const Weather* getCurrentWeather() const { return currentWeather; }

    /**
     * @brief Get current weather type.
     * @return WeatherType Current weather type.
     */
    WeatherType getCurrentWeatherType() const;

    /**
     * @brief Apply weather effects to a new tower.
     * @param tower Reference to the tower.
     */
    void applyWeatherToTower(Tower* tower);

    /**
     * @brief Apply weather effects to a new enemy.
     * @param enemy Reference to the enemy.
     */
    void applyWeatherToEnemy(Enemy* enemy);

    /**
     * @brief Remove weather effects from a tower.
     * @param tower Reference to the tower.
     */
    void removeWeatherFromTower(Tower* tower);

    /**
     * @brief Remove weather effects from an enemy.
     * @param enemy Reference to the enemy.
     */
    void removeWeatherFromEnemy(Enemy* enemy);

    /**
     * @brief Draw weather overlay.
     * @param target Render target.
     * @param states Render states.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    /**
     * @brief Initialize all weather instances.
     */
    void initializeWeatherInstances();

    /**
     * @brief Remove current weather effects from all entities.
     */
    void clearCurrentWeatherEffects();

    /**
     * @brief Apply current weather effects to all entities.
     */
    void applyCurrentWeatherEffects();
};