#include "Entity/Factory/TowerFactory.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

#include "Entity/Tower/TowerStat.hpp"
#include "Scene/Scene.hpp"

std::unique_ptr<Tower> TowerFactory::createFromConfigFile(
    const std::string& configPath, Scene& scene, const sf::Vector2f& position) {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        throw std::runtime_error("TowerFactory: Could not open config file: " +
                                 configPath);
    }

    nlohmann::json config;
    try {
        file >> config;
    } catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("TowerFactory: JSON parsing error in " +
                                 configPath + ": " + e.what());
    }

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

    // Parse and set cost
    if (config.contains("cost")) {
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
        std::string baseTexturePath, turretTexturePath;
        float textureWidth, textureHeight;
        parseTextures(config["texture"], baseTexturePath, turretTexturePath,
                      textureWidth, textureHeight);

        // Set texture paths
        if (!baseTexturePath.empty()) {
            builder.setBaseTexturePath(baseTexturePath);
        }

        if (!turretTexturePath.empty()) {
            builder.setTurretTexturePath(turretTexturePath);
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

    // TODO: Parse and add behaviors when behavior system is fully implemented
    // if (config.contains("behaviors")) {
    //     // Parse behaviors and add them to the builder
    // }

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
                                 std::string& baseTexturePath,
                                 std::string& turretTexturePath, float& width,
                                 float& height) {
    // Parse texture paths
    if (textureJson.contains("base")) {
        baseTexturePath = textureJson["base"].get<std::string>();
    }

    if (textureJson.contains("turret")) {
        turretTexturePath = textureJson["turret"].get<std::string>();
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
    if (!baseTexturePath.empty()) {
        std::cout << "TowerFactory: Base texture path: " << baseTexturePath
                  << std::endl;
    }

    if (!turretTexturePath.empty()) {
        std::cout << "TowerFactory: Turret texture path: " << turretTexturePath
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
}
