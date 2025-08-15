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
    std::unique_ptr<Weather> currentWeather;
    std::vector<WeatherType> waveWeatherPattern;
    int currentWaveIndex;
    
    // Cache for entities that have weather effects applied
    std::unordered_map<uint64_t, bool> affectedTowers;
    std::unordered_map<uint64_t, bool> affectedEnemies;

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
     * @brief Update weather effects.
     * @param deltaTime Time elapsed since last update.
     */
    void update(float deltaTime);

    /**
     * @brief Set weather pattern for all waves.
     * @param pattern Vector of weather types for each wave.
     */
    void setWeatherPattern(const std::vector<WeatherType>& pattern);

    /**
     * @brief Change weather for a specific wave.
     * @param waveIndex Index of the wave (0-based).
     */
    void changeWeatherForWave(int waveIndex);

    /**
     * @brief Get current weather.
     * @return const Weather* Pointer to current weather, or nullptr if none.
     */
    const Weather* getCurrentWeather() const { return currentWeather.get(); }

    /**
     * @brief Get current weather type.
     * @return WeatherType Current weather type.
     */
    WeatherType getCurrentWeatherType() const;

    /**
     * @brief Apply weather effects to a new tower.
     * @param tower Reference to the tower.
     */
    void applyWeatherToTower(Tower& tower);

    /**
     * @brief Apply weather effects to a new enemy.
     * @param enemy Reference to the enemy.
     */
    void applyWeatherToEnemy(Enemy& enemy);

    /**
     * @brief Remove weather effects from a tower.
     * @param tower Reference to the tower.
     */
    void removeWeatherFromTower(Tower& tower);

    /**
     * @brief Remove weather effects from an enemy.
     * @param enemy Reference to the enemy.
     */
    void removeWeatherFromEnemy(Enemy& enemy);

    /**
     * @brief Draw weather overlay.
     * @param target Render target.
     * @param states Render states.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    /**
     * @brief Create weather instance of specified type.
     * @param type Weather type to create.
     * @return std::unique_ptr<Weather> Created weather instance.
     */
    std::unique_ptr<Weather> createWeather(WeatherType type);

    /**
     * @brief Remove current weather effects from all entities.
     */
    void clearCurrentWeatherEffects();

    /**
     * @brief Apply current weather effects to all entities.
     */
    void applyCurrentWeatherEffects();
};