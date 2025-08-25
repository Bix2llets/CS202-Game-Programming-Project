/**
 * @file Difficulty.hpp
 * @brief Declares the Difficulty enum for game difficulty settings.
 */
#pragma once

#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <Core/JSONLoader.hpp> 
#include <Entity/Modules/EntityStat.hpp>
#include "Currency.hpp"
// Forward declarations
namespace sf {
    class Sprite;
}

class EntityStat;
class WeatherStat;

/**
 * @enum Difficulty
 * @brief Game difficulty levels.
 */
enum class Difficulty {
    Easy,   ///< Easiest difficulty
    Medium, ///< Normal difficulty
    Hard    ///< Hardest difficulty
};

/**
 * @class DifficultyLevel
 * @brief Represents a difficulty level configuration with modifiers for enemies and weather.
 */
class DifficultyLevel {
private:
    std::string name;
    std::string id;

    sf::Sprite icon;
    int index;
    EntityStat enemyModifier;
    std::unordered_map<std::string, WeatherStat> weatherModifiers;

    int maxHealth;

public:
    /**
     * @brief Default constructor.
     */
    DifficultyLevel();

    /**
     * @brief Copy constructor.
     */
    DifficultyLevel(const DifficultyLevel& other);

    /**
     * @brief Assignment operator.
     */
    DifficultyLevel& operator=(const DifficultyLevel& other);

    /**
     * @brief Destructor.
     */
    ~DifficultyLevel();

    /**
     * @brief Get the name of the difficulty level.
     * @return The difficulty level name.
     */
    std::string getName() const;

    /**
     * @brief Get the ID of the difficulty level.
     * @return The difficulty level ID.
     */
    std::string getId() const;

    /**
     * @brief Get the icon sprite for this difficulty level.
     * @return The icon sprite.
     */
    const sf::Sprite& getIcon() const;

    /**
     * @brief Get the weather modifier for a specific weather type.
     * @param weatherId The weather ID to get modifiers for.
     * @return Reference to the weather stat modifier.
     */
    WeatherStat& getWeatherModifier(const std::string& weatherId);

    /**
     * @brief Get the enemy modifier for this difficulty level.
     * @return Reference to the enemy stat modifier.
     */
    EntityStat& getEnemyModifier();

    /**
     * @brief Load difficulty configuration from JSON.
     * @param config The JSON configuration object.
     * @note Uses template to avoid including json headers in the header file.
     */
    void loadFromJson(const nlohmann::json& config);

    /**
     * @brief Load difficulty configuration from a config file by ID.
     * @param jsonID The JSON ID to load configuration for.
     */
    void loadFromConfigFile(const std::string& jsonID);

    inline int getIndex() const { return index; }
    inline int getMaxHealth() const { return maxHealth; }   
};