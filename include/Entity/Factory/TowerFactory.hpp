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
#include "Entity/Tower/Upgrades/UpgradeType.hpp"
#include "Entity/Tower/Upgrades/UpgradeDetails.hpp"
#include "Entity/Tower/Behaviors/TowerBehavior.hpp"

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
     * @param baseTextureId Output parameter for base texture path.
     * @param turretTextureId Output parameter for turret texture path.
     * @param width Output parameter for texture width.
     * @param height Output parameter for texture height.
     */
    static void parseTextures(
        const nlohmann::json& textureJson,
        std::string& baseTextureId,
        std::string& turretTextureId,
        float& width,
        float& height
    );
    
    /**
     * @brief Parse upgrade system configuration from JSON.
     * @param upgradesJson JSON object containing upgrades data.
     * @param builder TowerBuilder to configure with upgrades.
     */
    static void parseUpgrades(const nlohmann::json& upgradesJson, TowerBuilder& builder);
    
    /**
     * @brief Parse a single upgrade type from JSON.
     * @param typeId The upgrade type ID.
     * @param upgradeJson JSON object for this upgrade type.
     * @return std::unique_ptr<UpgradeType> Configured upgrade type.
     */
    static std::unique_ptr<UpgradeType> parseUpgradeType(int typeId, const nlohmann::json& upgradeJson);
    
    /**
     * @brief Parse upgrade details for a specific level.
     * @param detailsJson JSON object containing upgrade details.
     * @return UpgradeDetails Parsed upgrade details.
     */
    static UpgradeDetails parseUpgradeDetails(const nlohmann::json& detailsJson);
    
    /**
     * @brief Parse behaviors from JSON and add them to the builder.
     * @param behaviorsJson JSON object containing behaviors data.
     * @param builder TowerBuilder to configure with behaviors.
     */
    static void parseBehaviors(const nlohmann::json& behaviorsJson, TowerBuilder& builder);
    
    /**
     * @brief Parse combat behavior from JSON.
     * @param combatJson JSON object containing combat behavior data.
     * @return std::unique_ptr<CombatBehavior> Configured combat behavior.
     */
    static std::unique_ptr<CombatBehavior> parseCombatBehavior(const nlohmann::json& combatJson);
    
    /**
     * @brief Validate required fields in JSON configuration.
     * @param config JSON object to validate.
     * @throws std::runtime_error If required fields are missing.
     */
    static void validateConfig(const nlohmann::json& config);
};
