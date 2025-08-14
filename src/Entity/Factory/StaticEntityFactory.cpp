#include "Entity/Factory/StaticEntityFactory.hpp"

#include <stdexcept>

#include "Core/JSONLoader.hpp"
#include "Core/ResourceManager.hpp"
#include "Entity/StaticEntity/StaticEntity.hpp"
#include "Gameplay/Currency.hpp"
#include "Scene/Scene.hpp"
#include "Utility/logger.hpp"

std::unique_ptr<StaticEntity> StaticEntityFactory::createFromConfigFile(
    const std::string& jsonID, Scene& scene, const sf::Vector2f& position) {
    nlohmann::json config = JSONLoader::getInstance().getStaticEntity(jsonID);
    return createFromJson(config, scene, position);
}

std::unique_ptr<StaticEntity> StaticEntityFactory::createFromJson(
    const nlohmann::json& config, Scene& scene, const sf::Vector2f& position) {
    try {
        validateConfig(config);

        const std::string id = config["id"].get<std::string>();
        Logger::info("StaticEntityFactory: Creating static entity with ID: " + id);

        auto entity = std::make_unique<StaticEntity>(scene);

        // Set ID and texture name if present
        entity->id = id;
        if (config.contains("name")) {
            entity->name = config["name"].get<std::string>();
        } else {
            entity->name = id; // Use ID as name if name not specified
        }

        // Set dimensions
        if (config.contains("width")) {
            entity->setTextureWidth(config["width"].get<float>());
        }
        if (config.contains("height")) {
            entity->setTextureHeight(config["height"].get<float>());
        }

        // Parse remove cost
        if (config.contains("remove_cost")) {
            int scrap = 0, petroleum = 0;
            parseCurrency(config["remove_cost"], scrap, petroleum);
            Currency removeCost(scrap, petroleum);
            entity->setRemoveCost(removeCost);
        }
        
        // Parse texture
        if (config.contains("texture")) {
            parseTexture(config, *entity);
        }
        
        entity->setPosition(position);
        
        Logger::success("StaticEntityFactory: Successfully created static entity " + id);
        return entity;
    } catch (const nlohmann::json::exception& e) {
        Logger::error("StaticEntityFactory: JSON parsing error - " + std::string(e.what()));
        throw std::runtime_error("StaticEntityFactory: Failed to parse JSON configuration - " + std::string(e.what()));
    } catch (const std::exception& e) {
        Logger::error("StaticEntityFactory: Error creating static entity - " + std::string(e.what()));
        throw;
    }
}

void StaticEntityFactory::validateConfig(const nlohmann::json& config) {
    if (!config.is_object()) {
        Logger::error("StaticEntityFactory: Configuration must be a JSON object");
        throw std::runtime_error("StaticEntityFactory: Configuration must be a JSON object");
    }
    if (!config.contains("id") || !config["id"].is_string() || config["id"].get<std::string>().empty()) {
        Logger::error("StaticEntityFactory: Missing or invalid 'id'");
        throw std::runtime_error("StaticEntityFactory: Missing or invalid 'id'");
    }

    // Validate optional fields
    if (config.contains("name") && !config["name"].is_string()) {
        Logger::error("StaticEntityFactory: Field 'name' must be a string");
        throw std::runtime_error("StaticEntityFactory: Field 'name' must be a string");
    }
    if (config.contains("texture") && !config["texture"].is_string()) {
        Logger::error("StaticEntityFactory: Field 'texture' must be a string");
        throw std::runtime_error("StaticEntityFactory: Field 'texture' must be a string");
    }
    if (config.contains("width") && !config["width"].is_number()) {
        Logger::error("StaticEntityFactory: Field 'width' must be a number");
        throw std::runtime_error("StaticEntityFactory: Field 'width' must be a number");
    }
    if (config.contains("height") && !config["height"].is_number()) {
        Logger::error("StaticEntityFactory: Field 'height' must be a number");
        throw std::runtime_error("StaticEntityFactory: Field 'height' must be a number");
    }
    if (config.contains("remove_cost") && !config["remove_cost"].is_object()) {
        Logger::error("StaticEntityFactory: Field 'remove_cost' must be an object");
        throw std::runtime_error("StaticEntityFactory: Field 'remove_cost' must be an object");
    }

    // Validate remove_cost structure if present
    if (config.contains("remove_cost")) {
        const auto& cost = config["remove_cost"];
        if (cost.contains("scrap") && !cost["scrap"].is_number()) {
            Logger::error("StaticEntityFactory: Field 'remove_cost.scrap' must be a number");
            throw std::runtime_error("StaticEntityFactory: Field 'remove_cost.scrap' must be a number");
        }
        if (cost.contains("petroleum") && !cost["petroleum"].is_number()) {
            Logger::error("StaticEntityFactory: Field 'remove_cost.petroleum' must be a number");
            throw std::runtime_error("StaticEntityFactory: Field 'remove_cost.petroleum' must be a number");
        }
    }

    Logger::debug("StaticEntityFactory: Configuration validation passed");
}

void StaticEntityFactory::parseTexture(const nlohmann::json& config, StaticEntity& entity) {
    try {
        std::string textureId = config["texture"].get<std::string>();
        const sf::Texture* texture = ResourceManager::getInstance().getTexture(textureId);
        
        if (!texture) {
            Logger::error("StaticEntityFactory: Failed to load texture: " + textureId);
            throw std::runtime_error("StaticEntityFactory: Failed to load texture: " + textureId);
        }

        entity.loadSpriteTexture(*texture);
        Logger::debug("StaticEntityFactory: Successfully loaded texture: " + textureId);
    } catch (const std::exception& e) {
        Logger::error("StaticEntityFactory: Failed to parse texture configuration - " + std::string(e.what()));
        throw std::runtime_error("StaticEntityFactory: Failed to parse texture configuration - " + std::string(e.what()));
    }
}

void StaticEntityFactory::parseCurrency(const nlohmann::json& currencyJson, int& scrap, int& petroleum) {
    scrap = 0;
    petroleum = 0;
    
    if (currencyJson.contains("scrap")) {
        scrap = currencyJson["scrap"].get<int>();
    }
    if (currencyJson.contains("petroleum")) {
        petroleum = currencyJson["petroleum"].get<int>();
    }
}
