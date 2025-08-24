#include "Entity/Factory/ProjectileFactory.hpp"

#include "Entity/Tower/Projectile/Projectile.hpp"
#include "Entity/Tower/Projectile/FlightMode.hpp"
#include "Entity/Factory/AreaEffectFactory.hpp"
#include "Entity/Modules/SpriteAnimation.hpp"
#include "Core/JSONLoader.hpp"
#include "Utility/Logger.hpp"
#include "Scene/Scene.hpp"

std::unique_ptr<Projectile> ProjectileFactory::createFromConfigFile(
    const std::string& jsonID, Scene& scene) {

    nlohmann::json config = JSONLoader::getInstance().getProjectile(jsonID);
    
    return createFromJson(config, scene);
}

std::unique_ptr<Projectile> ProjectileFactory::createFromJson(const nlohmann::json& config, Scene& scene) {
    try {
        // Validate configuration first
        validateConfig(config);

        // Extract projectile ID
        std::string id = config["id"].get<std::string>();
        
        Logger::info("ProjectileFactory: * Creating projectile with ID: " + id);

        // Create projectile instance
        auto projectile = std::make_unique<Projectile>(scene, id);

        // Parse and set targeting type
        if (config.contains("targeting")) {
            int targetingType = parseTargeting(config["targeting"]);
            projectile->setTargetType(static_cast<ProjectileTargetType>(targetingType));
            Logger::debug("ProjectileFactory: Set targeting type for projectile " + id);
        }

        // Parse and set collision behavior
        if (config.contains("pierce_through_targets_count")) {
            int pierceCount = config["pierce_through_targets_count"].get<int>();
            projectile->setPierceCount(pierceCount);
            projectile->setCurrentPierceCount(0);
            Logger::debug("ProjectileFactory: Set pierce_through_targets_count to " + 
                         std::to_string(pierceCount) + " for projectile " + id);
        }

        // Parse and set collision distance
        if (config.contains("collision_distance")) {
            float collisionDistance = parseCollisionDistance(config);
            projectile->setCollisionDistance(collisionDistance);
            Logger::debug("ProjectileFactory: Set collision distance to " + 
                         std::to_string(collisionDistance) + " for projectile " + id);
        }

        // Parse rotate to target setting
        if (config.contains("rotate_to_target")) {
            bool rotateToTarget = config["rotate_to_target"].get<bool>();
            projectile->setRotateToTarget(rotateToTarget);
            Logger::debug("ProjectileFactory: Set rotate_to_target to " + 
                         std::string(rotateToTarget ? "true" : "false") + " for projectile " + id);
        }

        if(config.contains("on_hit_area_effects")) {
            for (const auto& effect : config["on_hit_area_effects"]) {
                if (!effect.is_string()) {
                    Logger::error("ProjectileFactory: Each element in 'on_hit_area_effects' must be a string");
                    throw std::runtime_error("ProjectileFactory: Each element in 'on_hit_area_effects' must be a string");
                }
                // JSONLoader::getInstance().getAllDifficulties
                std::string areaEffectId = effect.get<std::string>();
                std::unique_ptr<AreaEffect> areaEffect = AreaEffectFactory::createFromConfigFile(areaEffectId, scene, projectile->getUniqueId());
                if (areaEffect) {
                    projectile->addOnHitAreaEffect(std::move(areaEffect));
                    Logger::debug("ProjectileFactory: Added on-hit area effect for projectile " + areaEffectId);
                } else {
                    Logger::error("ProjectileFactory: Failed to create area effect for " + areaEffectId);
                }
            }
        }

        // Parse and set texture configuration
        if (config.contains("texture")) {
            parseTexture(config, *projectile);
        }

        // Create and set flight mode
        FlightMode* flightMode = createFlightMode(config);
        projectile->setFlightMode(flightMode); // Set the flight mode
        
        if (false) Logger::success("ProjectileFactory: Successfully created projectile " + id);
        return projectile;

    } catch (const nlohmann::json::exception& e) {
        if (false) Logger::error("ProjectileFactory: JSON parsing error - " + std::string(e.what()));
        throw std::runtime_error("ProjectileFactory: Failed to parse JSON configuration - " + std::string(e.what()));
    } catch (const std::exception& e) {
        if (false) Logger::error("ProjectileFactory: Error creating projectile - " + std::string(e.what()));
        throw; // Re-throw to preserve original exception
    }
}

void ProjectileFactory::validateConfig(const nlohmann::json& config) {
    // Check if config is an object
    if (!config.is_object()) {
        if (false) Logger::error("ProjectileFactory: Configuration must be a JSON object");
        throw std::runtime_error("ProjectileFactory: Configuration must be a JSON object");
    }

    // Validate required fields
    if (!config.contains("id")) {
        if (false) Logger::error("ProjectileFactory: Missing required field 'id'");
        throw std::runtime_error("ProjectileFactory: Missing required field 'id'");
    }

    if (!config["id"].is_string()) {
        if (false) Logger::error("ProjectileFactory: Field 'id' must be a string");
        throw std::runtime_error("ProjectileFactory: Field 'id' must be a string");
    }

    if (config["id"].get<std::string>().empty()) {
        if (false) Logger::error("ProjectileFactory: Field 'id' cannot be empty");
        throw std::runtime_error("ProjectileFactory: Field 'id' cannot be empty");
    }

    // Validate optional but typed fields
    if (config.contains("targeting")) {
        if (!config["targeting"].is_string()) {
            if (false) Logger::error("ProjectileFactory: Field 'targeting' must be a string");
            throw std::runtime_error("ProjectileFactory: Field 'targeting' must be a string");
        }
    }

    if (config.contains("stop_on_first_collision")) {
        if (!config["stop_on_first_collision"].is_boolean()) {
            if (false) Logger::error("ProjectileFactory: Field 'stop_on_first_collision' must be a boolean");
            throw std::runtime_error("ProjectileFactory: Field 'stop_on_first_collision' must be a boolean");
        }
    }

    if (config.contains("pierce_through_targets")) {
        if (!config["pierce_through_targets"].is_boolean()) {
            if (false) Logger::error("ProjectileFactory: Field 'pierce_through_targets' must be a boolean");
            throw std::runtime_error("ProjectileFactory: Field 'pierce_through_targets' must be a boolean");
        }
    }

    if (config.contains("collision_distance")) {
        if (!config["collision_distance"].is_number()) {
            if (false) Logger::error("ProjectileFactory: Field 'collision_distance' must be a number");
            throw std::runtime_error("ProjectileFactory: Field 'collision_distance' must be a number");
        }
        
        float distance = config["collision_distance"].get<float>();
        if (distance <= 0.0f) {
            if (false) Logger::error("ProjectileFactory: Field 'collision_distance' must be positive, got: " + std::to_string(distance));
            throw std::runtime_error("ProjectileFactory: Field 'collision_distance' must be positive, got: " + std::to_string(distance));
        }
    }

    if (config.contains("texture")) {
        if (!config["texture"].is_object()) {
            if (false) Logger::error("ProjectileFactory: Field 'texture' must be an object");
            throw std::runtime_error("ProjectileFactory: Field 'texture' must be an object");
        }
    }

    if (config.contains("rotate_to_target")) {
        if (!config["rotate_to_target"].is_boolean()) {
            if (false) Logger::error("ProjectileFactory: Field 'rotate_to_target' must be a boolean");
            throw std::runtime_error("ProjectileFactory: Field 'rotate_to_target' must be a boolean");
        }
    }

    if (config.contains("on_hit_area_effects")) {
        if (!config["on_hit_area_effects"].is_array()) {
            if (false) Logger::error("ProjectileFactory: Field 'on_hit_area_effects' must be an array");
            throw std::runtime_error("ProjectileFactory: Field 'on_hit_area_effects' must be an array");
        }
    }

    if (false) Logger::debug("ProjectileFactory: Configuration validation passed");
}

int ProjectileFactory::parseTargeting(const nlohmann::json& targetingValue) {
    std::string targeting = targetingValue.get<std::string>();
    
    if (targeting == "enemy" || targeting == "target_entity") {
        return 1; // ProjectileTargetType::TargetEntity
    } else if (targeting == "location" || targeting == "target_location") {
        return 2; // ProjectileTargetType::TargetLocation
    } else if (targeting == "trajectory") {
        return 0; // ProjectileTargetType::Trajectory
    } else {
        if (false) Logger::warning("ProjectileFactory: Unknown targeting type '" + targeting + "', defaulting to trajectory");
        return 0; // Default to trajectory
    }
}

FlightMode* ProjectileFactory::createFlightMode(const nlohmann::json& config) {
    // Default to linear flight mode
    std::string flightType = "linear";
    
    // Check if flight mode is specified in targeting or separate field
    if (config.contains("flight_mode")) {
        if (config["flight_mode"].is_string()) {
            flightType = config["flight_mode"].get<std::string>();
        } else {
            if (false) Logger::warning("ProjectileFactory: Field 'flight_mode' should be a string, using default 'linear'");
        }
    }

    if (flightType == "linear") {
        if (false) Logger::debug("ProjectileFactory: Creating LinearFlightMode");
        return new LinearFlightMode();
    } else {
        if (false) Logger::warning("ProjectileFactory: Unsupported flight mode type '" + flightType + "', using LinearFlightMode");
        return new LinearFlightMode();
    }
}

float ProjectileFactory::parseCollisionDistance(const nlohmann::json& config) {
    float distance = config["collision_distance"].get<float>();
    
    // Additional validation beyond what's done in validateConfig
    if (distance > 30.0f) {
        if (false) Logger::warning("ProjectileFactory: Collision distance " + std::to_string(distance) + 
                       " is unusually large, consider reviewing configuration");
    }
    
    return distance;
}

void ProjectileFactory::parseTexture(const nlohmann::json& config, Projectile& projectile) {
    try {
        const auto& textureConfig = config["texture"];

        // Load the sprite animation using the Projectile's loadSpriteAnimation method
        projectile.loadSpriteAnimation(textureConfig);
        if (false) Logger::debug("ProjectileFactory: Successfully loaded sprite animation for projectile");
        
    } catch (const std::exception& e) {
        if (false) Logger::error("ProjectileFactory: Failed to parse texture configuration - " + std::string(e.what()));
        throw std::runtime_error("ProjectileFactory: Failed to parse texture configuration - " + std::string(e.what()));
    }
}
