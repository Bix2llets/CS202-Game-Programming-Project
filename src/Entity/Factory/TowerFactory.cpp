#include "Entity/Factory/TowerFactory.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <memory>


#include "Entity/Modules/EntityStat.hpp"
#include "Entity/Tower/Upgrades/UpgradeType.hpp"
#include "Entity/Tower/Upgrades/UpgradeDetails.hpp"
#include "Entity/Tower/Behaviors/TowerBehavior.hpp"
#include "Entity/Tower/Behaviors/Combat/FireMode.hpp"
#include "Entity/Tower/Behaviors/Combat/TargetSelector.hpp"
#include "Scene/Scene.hpp"
#include "Core/JSONLoader.hpp"
#include "Utility/logger.hpp"
#include "Entity/Factory/ProjectileFactory.hpp"

std::unique_ptr<Tower> TowerFactory::createFromConfigFile(
    const std::string& jsonID, Scene& scene, const sf::Vector2f& position) {

    nlohmann::json config = JSONLoader::getInstance().getTower(jsonID);

    return createFromJson(config, scene, position);
}

std::unique_ptr<Tower> TowerFactory::createFromJson(
    const nlohmann::json& config, Scene& scene, const sf::Vector2f& position) {
    validateConfig(config);

    TowerBuilder builder;

    // Set required fields
    builder.setId(config["id"].get<std::string>());

    // Set basic properties
    if (config.contains("name")) {
        builder.setName(config["name"].get<std::string>());
    }

    if (config.contains("description")) {
        builder.setDescription(config["description"].get<std::string>());
    }

    if (config.contains("buildable")) {
        builder.setBuildable(config["buildable"].get<bool>());
    }

    // Only parse cost and upgrades for buildable towers
    bool isBuildable = config.value("buildable", true); // Default to true if not specified
    
    // Parse and set cost (only for buildable towers)
    if (isBuildable && config.contains("cost")) {
        Currency cost = parseCost(config["cost"]);
        builder.setCost(cost);
    }

    // Parse and set stats
    if (config.contains("stats")) {
        auto stats = parseStats(config["stats"]);
        builder.setStats(std::move(stats));
    }

    // Parse and set texture paths and dimensions
    if (config.contains("texture")) {
        std::string baseTextureId;
        nlohmann::json turretAnimationPath;
        float textureWidth, textureHeight;
        
        parseTextures(config["texture"], baseTextureId, textureWidth, textureHeight);

        // Set texture paths
        if (!baseTextureId.empty()) {
            builder.setBaseTexturePath(baseTextureId);
        }

        if (config["texture"].contains("turret_animation")) {
            builder.setTurretAnimationPath(config["texture"]["turret_animation"]);
        }

        // Set texture dimensions
        builder.setTextureDimensions(textureWidth, textureHeight);
    }

    // Set timer interval based on fire_rate if available
    if (config.contains("stats") && config["stats"].contains("fire_rate")) {
        float fireRate = config["stats"]["fire_rate"].get<float>();
        if (fireRate > 0) {
            builder.setTimerInterval(
                1.0f / fireRate);  // Convert fire rate to interval
        }
    }

    // Parse and configure upgrade system (only for buildable towers)
    if (isBuildable && config.contains("upgrades")) {
        parseUpgrades(config["upgrades"], builder);
    }

    // Parse and add behaviors
    if (config.contains("behaviors")) {
        parseBehaviors(config["behaviors"], builder, scene);
    }

    return builder.setScene(scene).setPosition(position).build();
}

Currency TowerFactory::parseCost(const nlohmann::json& costJson) {
    int scraps = 0;
    int petroleum = 0;

    if (costJson.contains("scrap")) {
        scraps = costJson["scrap"].get<int>();
    }

    if (costJson.contains("petroleum")) {
        petroleum = costJson["petroleum"].get<int>();
    }

    return Currency(scraps, petroleum);
}

std::unique_ptr<EntityStat> TowerFactory::parseStats(
    const nlohmann::json& statsJson) {
    auto stats = std::make_unique<EntityStat>();

    // Parse all stats from the JSON
    for (auto it = statsJson.begin(); it != statsJson.end(); ++it) {
        const std::string& statName = it.key();
        float statValue = it.value().get<float>();

        stats->setStat(statName, statValue);
    }

    return stats;
}

void TowerFactory::parseTextures(const nlohmann::json& textureJson,
                                 std::string& baseTextureId,
                                 float& width,
                                 float& height) {
    // Parse texture paths
    if (textureJson.contains("base")) {
        baseTextureId = textureJson["base"].get<std::string>();
    }

    // Parse texture dimensions with defaults
    if (textureJson.contains("width")) {
        width = textureJson["width"].get<float>();
    } else {
        width = 32.0f;  // Default width
    }

    if (textureJson.contains("height")) {
        height = textureJson["height"].get<float>();
    } else {
        height = 32.0f;  // Default height
    }

    // Log texture information for debugging
    if (!baseTextureId.empty()) {
        // Logger::debug("TowerFactory: Base texture path: " + baseTextureId);
    }

    // Logger::debug("TowerFactory: Texture dimensions: " + std::to_string(width) + "x" + std::to_string(height));
}

void TowerFactory::validateConfig(const nlohmann::json& config) {
    // Check for required fields
    if (!config.contains("id")) {

        // Print out config for debugging
        std::cerr << "TowerFactory: Missing required field 'id' in tower configuration" << std::endl;
        std::cerr << "TowerFactory: Configuration: " << config.dump(4) << std::endl;

        throw std::runtime_error(
            "TowerFactory: Missing required field 'id' in tower configuration");
    }

    if (!config["id"].is_string()) {
        throw std::runtime_error("TowerFactory: Field 'id' must be a string");
    }

    // Validate optional fields if they exist
    if (config.contains("name") && !config["name"].is_string()) {
        throw std::runtime_error("TowerFactory: Field 'name' must be a string");
    }

    if (config.contains("description") && !config["description"].is_string()) {
        throw std::runtime_error(
            "TowerFactory: Field 'description' must be a string");
    }

    if (config.contains("buildable") && !config["buildable"].is_boolean()) {
        throw std::runtime_error(
            "TowerFactory: Field 'buildable' must be a boolean");
    }

    if (config.contains("cost") && !config["cost"].is_object()) {
        throw std::runtime_error(
            "TowerFactory: Field 'cost' must be an object");
    }

    if (config.contains("stats") && !config["stats"].is_object()) {
        throw std::runtime_error(
            "TowerFactory: Field 'stats' must be an object");
    }

    if (config.contains("texture") && !config["texture"].is_object()) {
        throw std::runtime_error(
            "TowerFactory: Field 'texture' must be an object");
    }

    if (config.contains("upgrades") && !config["upgrades"].is_object()) {
        throw std::runtime_error(
            "TowerFactory: Field 'upgrades' must be an object");
    }

    if (config.contains("behaviors") && !config["behaviors"].is_object()) {
        throw std::runtime_error(
            "TowerFactory: Field 'behaviors' must be an object");
    }
}

void TowerFactory::parseUpgrades(const nlohmann::json& upgradesJson, TowerBuilder& builder) {
    // Set maximum total upgrades
    if (upgradesJson.contains("max_total_upgrades")) {
        int maxUpgrades = upgradesJson["max_total_upgrades"].get<int>();
        builder.setMaxTotalUpgrades(maxUpgrades);
    }

    // Parse individual upgrade types
    for (auto it = upgradesJson.begin(); it != upgradesJson.end(); ++it) {
        const std::string& key = it.key();
        
        // Skip non-numeric keys (like "max_total_upgrades", "upgrade_types")
        if (key == "max_total_upgrades" || key == "upgrade_types") {
            continue;
        }
        
        try {
            int typeId = std::stoi(key);
            auto upgradeType = parseUpgradeType(typeId, it.value());
            builder.addUpgradeType(typeId, std::move(upgradeType));
        } catch (const std::invalid_argument&) {
            // Skip non-numeric keys
            continue;
        }
    }
}

std::unique_ptr<UpgradeType> TowerFactory::parseUpgradeType(int typeId, const nlohmann::json& upgradeJson) {
    // Extract basic upgrade type information
    std::string displayName = upgradeJson.value("display_name", "");
    std::string description = upgradeJson.value("description", "");
    int maxLevel = upgradeJson.value("max_level", 1);
    std::string iconID = upgradeJson.value("icon", "");
    std::string evolveTo = upgradeJson.value("evolve_to", "");
    
    // Create the upgrade type
    auto upgradeType = std::make_unique<UpgradeType>(displayName, description, maxLevel, iconID, evolveTo);
    
    // Parse upgrade details for each level
    if (upgradeJson.contains("upgrade_details")) {
        const auto& detailsJson = upgradeJson["upgrade_details"];
        
        for (auto it = detailsJson.begin(); it != detailsJson.end(); ++it) {
            try {
                int level = std::stoi(it.key());
                UpgradeDetails details = parseUpgradeDetails(it.value());
                upgradeType->addLevelDetails(level, details);
            } catch (const std::invalid_argument&) {
                // Skip non-numeric level keys
                continue;
            }
        }
    }
    
    return upgradeType;
}

UpgradeDetails TowerFactory::parseUpgradeDetails(const nlohmann::json& detailsJson) {
    // Parse cost
    Currency cost(0, 0);
    if (detailsJson.contains("scrap")) {
        cost = Currency(detailsJson["scrap"].get<int>(), cost.getPetroleum().value);
    }
    if (detailsJson.contains("petroleum")) {
        cost = Currency(cost.getScraps().value, detailsJson["petroleum"].get<int>());
    }
    
    // Parse bonus stats
    EntityStat bonusStats;
    if (detailsJson.contains("bonus_stats")) {
        const auto& bonusJson = detailsJson["bonus_stats"];
        for (auto it = bonusJson.begin(); it != bonusJson.end(); ++it) {
            const std::string& statName = it.key();
            float statValue = it.value().get<float>();
            bonusStats.setStat(statName, statValue);
        }
    }
    
    return UpgradeDetails(cost, bonusStats);
}

void TowerFactory::parseBehaviors(const nlohmann::json& behaviorsJson, TowerBuilder& builder, Scene& scene) {
    // Parse combat behavior
    if (behaviorsJson.contains("combat")) {
        const auto& combatJson = behaviorsJson["combat"];
        auto combatBehavior = parseCombatBehavior(combatJson, scene);
        if (combatBehavior) {
            builder.addBehavior(std::move(combatBehavior));
        }
    }
    
    // TODO: Parse other behavior types when implemented
    // if (behaviorsJson.contains("resource")) {
    //     const auto& resourceJson = behaviorsJson["resource"];
    //     auto resourceBehavior = parseResourceBehavior(resourceJson);
    //     builder.addBehavior(std::move(resourceBehavior));
    // }
    //
    // if (behaviorsJson.contains("glowing")) {
    //     const auto& glowingJson = behaviorsJson["glowing"];
    //     auto glowingBehavior = parseGlowingBehavior(glowingJson);
    //     builder.addBehavior(std::move(glowingBehavior));
    // }
}

std::unique_ptr<CombatBehavior> TowerFactory::parseCombatBehavior(const nlohmann::json& combatJson, Scene& scene) {
    // Parse fire mode type
    std::string fireType = combatJson.value("type", "instant");
    
    // Create fire mode based on type
    Combat::FireMode* fireMode = nullptr;
    if (fireType == "instant") {
        fireMode = new Combat::InstantFireMode();
    } else if (fireType == "projectile") {
        Combat::ProjectileFireMode* projectileFireMode = new Combat::ProjectileFireMode();
        
        if (!combatJson.contains("projectile")) {
            // Logger::warning("TowerFactory: Fire mode type 'projectile' requires a 'projectile' field. Skipping combat behavior.");
            delete projectileFireMode; // Clean up if not used
            return nullptr;
        }

        std::string projectileId = combatJson["projectile"].get<std::string>();
        std::unique_ptr<Projectile> projectile = ProjectileFactory::createFromConfigFile(projectileId, scene);
        projectileFireMode->setProjectile(std::move(projectile));

        fireMode = projectileFireMode;
    } else {
        return nullptr;
    }
    
    // Parse targeting strategy
    std::string targeting = combatJson.value("targeting", "nearest");
    
    // Create target selector based on targeting strategy
    Combat::TargetSelector* targetSelector = nullptr;
    if (targeting == "nearest") {
        targetSelector = new Combat::NearestTargetSelector();
    } else if (targeting == "farthest" || targeting == "furthest") {
        targetSelector = new Combat::FarthestTargetSelector();
    } else if (targeting == "lowest" || targeting == "lowest_health" || targeting == "weakest") {
        targetSelector = new Combat::LowestHealthTargetSelector();
    } else if (targeting == "highest" || targeting == "highest_health" || targeting == "strongest") {
        targetSelector = new Combat::HighestHealthTargetSelector();
    } else {
        // For other targeting strategies, default to nearest for now
        Logger::warning("TowerFactory: Targeting strategy '" + targeting + "' not found. Using nearest targeting.");
        targetSelector = new Combat::NearestTargetSelector();
    }
    

    return std::make_unique<CombatBehavior>(nullptr, targetSelector, fireMode);
}
