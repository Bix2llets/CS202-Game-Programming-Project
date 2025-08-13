#include "Entity/Factory/AreaEffectFactory.hpp"

#include <stdexcept>

#include "Core/JSONLoader.hpp"
#include "Entity/AreaEffect/AreaEffect.hpp"
#include "Entity/Modules/EntityStat.hpp"
#include "Scene/Scene.hpp"
#include "Utility/logger.hpp"

std::unique_ptr<AreaEffect> AreaEffectFactory::createFromConfigFile(
    const std::string& jsonID, Scene& scene, uint64_t sourceId) {
    nlohmann::json config = JSONLoader::getInstance().getAreaEffect(jsonID);
    return createFromJson(config, scene, sourceId);
}

std::unique_ptr<AreaEffect> AreaEffectFactory::createFromJson(
    const nlohmann::json& config, Scene& scene, uint64_t sourceId) {
    try {
        validateConfig(config);

        const std::string id = config["id"].get<std::string>();
        Logger::info("AreaEffectFactory: Creating area-effect with ID: " + id);

        auto effect = std::make_unique<AreaEffect>(scene, sourceId);

        // Stats block
        if (config.contains("stats")) {
            const auto& statsJson = config["stats"];
            for (auto it = statsJson.begin(); it != statsJson.end(); ++it) {
                const std::string statName = it.key();
                float statValue = it.value().get<float>();
                effect->getStats().setStat(statName, statValue);
            }
        }

        // Damage type
        if (config.contains("damage_type")) {
            effect->setDamageType(static_cast<DamageType>(parseDamageType(config["damage_type"])));
        }

        // Texture / animation
        if (config.contains("texture")) {
            parseTexture(config, *effect);
        }

        // Setup timers and derived fields
        effect->setUp();

        Logger::success("AreaEffectFactory: Successfully created area-effect " + id);
        return effect;
    } catch (const nlohmann::json::exception& e) {
        Logger::error("AreaEffectFactory: JSON parsing error - " + std::string(e.what()));
        throw std::runtime_error("AreaEffectFactory: Failed to parse JSON configuration - " + std::string(e.what()));
    } catch (const std::exception& e) {
        Logger::error("AreaEffectFactory: Error creating area-effect - " + std::string(e.what()));
        throw;
    }
}

void AreaEffectFactory::validateConfig(const nlohmann::json& config) {
    if (!config.is_object()) {
        Logger::error("AreaEffectFactory: Configuration must be a JSON object");
        throw std::runtime_error("AreaEffectFactory: Configuration must be a JSON object");
    }
    if (!config.contains("id") || !config["id"].is_string() || config["id"].get<std::string>().empty()) {
        Logger::error("AreaEffectFactory: Missing or invalid 'id'");
        throw std::runtime_error("AreaEffectFactory: Missing or invalid 'id'");
    }

    if (config.contains("stats") && !config["stats"].is_object()) {
        Logger::error("AreaEffectFactory: Field 'stats' must be an object");
        throw std::runtime_error("AreaEffectFactory: Field 'stats' must be an object");
    }
    if (config.contains("texture") && !config["texture"].is_object()) {
        Logger::error("AreaEffectFactory: Field 'texture' must be an object");
        throw std::runtime_error("AreaEffectFactory: Field 'texture' must be an object");
    }
    if (config.contains("damage_type") && !config["damage_type"].is_string()) {
        Logger::error("AreaEffectFactory: Field 'damage_type' must be a string");
        throw std::runtime_error("AreaEffectFactory: Field 'damage_type' must be a string");
    }

    // Validate some expected stats if present
    if (config.contains("stats")) {
        const auto& s = config["stats"];
        auto checkNumber = [&](const char* key) {
            if (s.contains(key) && !s[key].is_number()) {
                Logger::error(std::string("AreaEffectFactory: Stat '") + key + "' must be a number");
                throw std::runtime_error(std::string("AreaEffectFactory: Stat '") + key + "' must be a number");
            }
        };
        checkNumber("texture_width");
        checkNumber("texture_height");
        checkNumber("radius");
        checkNumber("damage");
        checkNumber("duration");
        checkNumber("repeating_per_interval");
        checkNumber("repeating_times");
    }

    Logger::debug("AreaEffectFactory: Configuration validation passed");
}

int AreaEffectFactory::parseDamageType(const nlohmann::json& value) {
    std::string type = value.get<std::string>();
    if (type == "physical") return static_cast<int>(DamageType::Physical);
    if (type == "fire") return static_cast<int>(DamageType::Fire);
    if (type == "napalm") return static_cast<int>(DamageType::Napalm);
    Logger::warning("AreaEffectFactory: Unknown damage_type '" + type + "', defaulting to physical");
    return static_cast<int>(DamageType::Physical);
}

void AreaEffectFactory::parseTexture(const nlohmann::json& config, AreaEffect& effect) {
    try {
        const auto& textureConfig = config["texture"];
        // AreaEffect exposes loadSpriteAnimation like Projectile
        effect.loadSpriteAnimation(textureConfig);
        Logger::debug("AreaEffectFactory: Successfully loaded sprite animation for area-effect");
    } catch (const std::exception& e) {
        Logger::error("AreaEffectFactory: Failed to parse texture configuration - " + std::string(e.what()));
        throw std::runtime_error("AreaEffectFactory: Failed to parse texture configuration - " + std::string(e.what()));
    }
}
