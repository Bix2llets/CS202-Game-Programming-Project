/**
 * @file Difficulty.cpp
 * @brief Implementation of the DifficultyLevel class.
 */

#include "Gameplay/Difficulty.hpp"
#include <Base/Constants.hpp>
#include <Utility/Logger.hpp>
#include <Core/ResourceManager.hpp>
#include <Gameplay/Weather/Weather.hpp>

DifficultyLevel::DifficultyLevel() 
    : icon(GameConstants::BLANK_TEXTURE)
    , enemyModifier()
{
}

DifficultyLevel::DifficultyLevel(const DifficultyLevel& other)
    : name(other.name)
    , id(other.id)
    , icon(other.icon)
    , enemyModifier(other.enemyModifier)
{
    // Deep copy weather modifiers
    for (const auto& [weatherId, weatherStat] : other.weatherModifiers) {
        weatherModifiers[weatherId] = weatherStat;
    }
}

DifficultyLevel& DifficultyLevel::operator=(const DifficultyLevel& other) {
    if (this != &other) {
        name = other.name;
        id = other.id;
        icon = other.icon;
        enemyModifier = other.enemyModifier;

        // Deep copy weather modifiers
        weatherModifiers.clear();
        for (const auto& [weatherId, weatherStat] : other.weatherModifiers) {
            weatherModifiers[weatherId] = weatherStat;
        }
    }
    return *this;
}

DifficultyLevel::~DifficultyLevel() = default;

std::string DifficultyLevel::getName() const {
    return name;
}

std::string DifficultyLevel::getId() const {
    return id;
}

const sf::Sprite& DifficultyLevel::getIcon() const {
    return icon;
}

WeatherStat& DifficultyLevel::getWeatherModifier(const std::string& weatherId) {
    if (weatherModifiers.find(weatherId) == weatherModifiers.end()) {
        weatherModifiers[weatherId] = WeatherStat();
    }
    return weatherModifiers[weatherId];
}

EntityStat& DifficultyLevel::getEnemyModifier() {
    return enemyModifier;
}

void DifficultyLevel::loadFromJson(const nlohmann::json& config) {
    if (config.contains("id")) {
        id = config["id"];
    } else {
        Logger::critical("DifficultyLevel: Missing 'id' field");
    }

    if (config.contains("name")) {
        name = config["name"];
    } else {
        name = id;
        Logger::warning("DifficultyLevel: Missing 'name' field");
    }

    if(config.contains("icon")) {
        if (config["icon"].is_string()) {
            icon.setTexture(*ResourceManager::getInstance().getTexture(config["icon"].get<std::string>()));
        } else {
            icon = sf::Sprite(GameConstants::BLANK_TEXTURE);
            Logger::warning("DifficultyLevel: 'icon' field is not a string");
        }
    } else {
        icon = sf::Sprite(GameConstants::BLANK_TEXTURE);
        Logger::warning("DifficultyLevel: 'icon' field is not a string");
    }
    
    if (config.contains("icon")) {
        icon.setTexture(*ResourceManager::getInstance().getTexture(config["icon"].get<std::string>()));
    } else {
        Logger::warning("DifficultyLevel: Missing 'icon' field");
    }

    if (config.contains("enemy_modifiers")) {
        enemyModifier.loadFromJson(config["enemy_modifiers"]);
    }

    if (config.contains("weather_modifiers")) {
        for (const auto& [weatherId, weatherConfig] : config["weather_modifiers"].items()) {
            if (weatherModifiers.find(weatherId) == weatherModifiers.end()) {
                weatherModifiers[weatherId] = WeatherStat();
            }
            weatherModifiers[weatherId].loadFromJson(weatherConfig);
        }
    }
}

void DifficultyLevel::loadFromConfigFile(const std::string& jsonID) {
    nlohmann::json config = JSONLoader::getInstance().getDifficulty(jsonID);
    loadFromJson(config);
}
