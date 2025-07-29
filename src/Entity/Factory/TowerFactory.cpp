#include "Entity/Factory/TowerFactory.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

#include "Entity/Tower/TowerStat.hpp"
#include "Entity/Tower/Upgrades/UpgradeType.hpp"
#include "Entity/Tower/Upgrades/UpgradeDetails.hpp"
#include "Entity/Tower/Behaviors/TowerBehavior.hpp"
#include "Entity/Tower/Behaviors/Combat/FireMode.hpp"
#include "Entity/Tower/Behaviors/Combat/TargetSelector.hpp"
#include "Scene/Scene.hpp"
#include "Core/JSONLoader.hpp"

std::unique_ptr<Tower> TowerFactory::createFromConfigFile(
    const std::string& jsonID, Scene& scene, const sf::Vector2f& position) {

    nlohmann::json config = JSONLoader::getInstance().getTower(jsonID);

    return createFromJson(config, scene, position);
}

std::unique_ptr<Tower> TowerFactory::createFromJson(
    const nlohmann::json& config, Scene& scene, const sf::Vector2f& position) {
    validateConfig(config);

    TowerBuilder builder = builderFromJson(config);
    return builder.setScene(scene).setPosition(position).build();
}

TowerBuilder TowerFactory::builderFromJson(const nlohmann::json& config) {
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
        std::string baseTextureId, turretTextureId;
        float textureWidth, textureHeight;
        parseTextures(config["texture"], baseTextureId, turretTextureId,
                      textureWidth, textureHeight);

        // Set texture paths
        if (!baseTextureId.empty()) {
            builder.setBaseTexturePath(baseTextureId);
        }

        if (!turretTextureId.empty()) {
            builder.setTurretTexturePath(turretTextureId);
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
        parseBehaviors(config["behaviors"], builder);
    }

    return builder;
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

std::unique_ptr<TowerStat> TowerFactory::parseStats(
    const nlohmann::json& statsJson) {
    auto stats = std::make_unique<TowerStat>();

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
                                 std::string& turretTextureId, float& width,
                                 float& height) {
    // Parse texture paths
    if (textureJson.contains("base")) {
        baseTextureId = textureJson["base"].get<std::string>();
    }

    if (textureJson.contains("turret")) {
        turretTextureId = textureJson["turret"].get<std::string>();
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
        std::cout << "TowerFactory: Base texture path: " << baseTextureId
                  << std::endl;
    }

    if (!turretTextureId.empty()) {
        std::cout << "TowerFactory: Turret texture path: " << turretTextureId
                  << std::endl;
    }

    std::cout << "TowerFactory: Texture dimensions: " << width << "x" << height
              << std::endl;
}

void TowerFactory::validateConfig(const nlohmann::json& config) {
    // Check for required fields
    if (!config.contains("id")) {
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
    std::string iconPath = upgradeJson.value("icon", "");
    std::string evolveTo = upgradeJson.value("evolve_to", "");
    
    // Create the upgrade type
    auto upgradeType = std::make_unique<UpgradeType>(displayName, description, maxLevel, iconPath, evolveTo);
    
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
    TowerStat bonusStats;
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

void TowerFactory::parseBehaviors(const nlohmann::json& behaviorsJson, TowerBuilder& builder) {
    // Parse combat behavior
    if (behaviorsJson.contains("combat")) {
        const auto& combatJson = behaviorsJson["combat"];
        auto combatBehavior = parseCombatBehavior(combatJson);
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

std::unique_ptr<CombatBehavior> TowerFactory::parseCombatBehavior(const nlohmann::json& combatJson) {
    // Parse fire mode type
    std::string fireType = combatJson.value("type", "instant");
    
    // Create fire mode based on type
    Combat::FireMode* fireMode = nullptr;
    if (fireType == "instant") {
        fireMode = new Combat::InstantFireMode();
    } else {
        // For other fire mode types, do nothing yet as requested
        std::cout << "TowerFactory: Fire mode type '" << fireType << "' not yet implemented. Skipping combat behavior." << std::endl;
        return nullptr;
    }
    
    // Parse targeting strategy
    std::string targeting = combatJson.value("targeting", "nearest");
    
    // Create target selector based on targeting strategy
    Combat::TargetSelector* targetSelector = nullptr;
    if (targeting == "nearest") {
        targetSelector = new Combat::NearestTargetSelector();
    } else {
        // For other targeting strategies, default to nearest for now
        std::cout << "TowerFactory: Targeting strategy '" << targeting << "' not fully implemented. Using nearest targeting." << std::endl;
        targetSelector = new Combat::NearestTargetSelector();
    }
    
    // Create and return combat behavior
    // Note: CombatBehavior constructor takes ownership of the pointers
    return std::make_unique<CombatBehavior>(nullptr, targetSelector, fireMode);
}
