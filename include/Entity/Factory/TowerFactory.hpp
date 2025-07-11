/**
 * @file TowerFactory.hpp
 * @brief Declares the TowerFactory class for creating tower entities from JSON configuration.
 *
 * The TowerFactory centralizes the creation of towers, loading configuration from JSON files
 * and using TowerBuilder to construct fully configured Tower objects.
 */
#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "json.hpp"

#include "Entity/Tower/Tower.hpp"
#include "Entity/Tower/TowerBuilder.hpp"

class Scene;

/**
 * @class TowerFactory
 * @brief Factory class for creating Tower entities from JSON configuration files.
 *
 * Handles loading tower configurations from JSON, parsing the data, and using
 * TowerBuilder to create fully configured Tower objects.
 */
class TowerFactory {
public:
    /**
     * @brief Create a tower from a JSON configuration file.
     * @param configPath Path to the JSON configuration file.
     * @param scene Reference to the game scene.
     * @param position Position to place the tower (overrides JSON position if any).
     * @return std::unique_ptr<Tower> Newly created tower.
     * @throws std::runtime_error If configuration loading or parsing fails.
     */
    static std::unique_ptr<Tower> createFromConfigFile(
        const std::string& configPath, 
        Scene& scene, 
        const sf::Vector2f& position = sf::Vector2f(0, 0)
    );
    
    /**
     * @brief Create a tower from JSON data.
     * @param config JSON object containing tower configuration.
     * @param scene Reference to the game scene.
     * @param position Position to place the tower.
     * @return std::unique_ptr<Tower> Newly created tower.
     * @throws std::runtime_error If JSON parsing fails.
     */
    static std::unique_ptr<Tower> createFromJson(
        const nlohmann::json& config, 
        Scene& scene, 
        const sf::Vector2f& position = sf::Vector2f(0, 0)
    );
    
    /**
     * @brief Create a TowerBuilder configured from JSON data.
     * @param config JSON object containing tower configuration.
     * @return TowerBuilder Configured builder ready to build a tower.
     */
    static TowerBuilder builderFromJson(const nlohmann::json& config);

private:
    /**
     * @brief Parse cost information from JSON.
     * @param costJson JSON object containing cost data.
     * @return Currency Parsed currency object.
     */
    static Currency parseCost(const nlohmann::json& costJson);
    
    /**
     * @brief Parse stats information from JSON.
     * @param statsJson JSON object containing stats data.
     * @return std::unique_ptr<TowerStat> Parsed tower statistics.
     */
    static std::unique_ptr<TowerStat> parseStats(const nlohmann::json& statsJson);
    
    /**
     * @brief Parse texture paths and dimensions from JSON.
     * @param textureJson JSON object containing texture data.
     * @param baseTexturePath Output parameter for base texture path.
     * @param turretTexturePath Output parameter for turret texture path.
     * @param width Output parameter for texture width.
     * @param height Output parameter for texture height.
     */
    static void parseTextures(
        const nlohmann::json& textureJson,
        std::string& baseTexturePath,
        std::string& turretTexturePath,
        float& width,
        float& height
    );
    
    /**
     * @brief Validate required fields in JSON configuration.
     * @param config JSON object to validate.
     * @throws std::runtime_error If required fields are missing.
     */
    static void validateConfig(const nlohmann::json& config);
};
