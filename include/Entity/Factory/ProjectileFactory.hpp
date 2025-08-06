/**
 * @file ProjectileFactory.hpp
 * @brief Declares the ProjectileFactory class for creating Projectile entities from JSON configuration.
 *
 * The ProjectileFactory handles loading projectile configurations from JSON data,
 * parsing and validating the configuration, and creating fully configured Projectile objects.
 */
#pragma once

#include <json.hpp>
#include <memory>
#include <string>

// Forward declarations
class Projectile;
class Scene;
class FlightMode;

/**
 * @class ProjectileFactory
 * @brief Factory class for creating Projectile entities from JSON configuration.
 *
 * Handles loading projectile configurations from JSON, parsing the data, and creating
 * fully configured Projectile objects with appropriate flight modes, targeting behaviors,
 * and collision settings.
 */
class ProjectileFactory {
public:
    /**
     * @brief Create a Projectile from a configuration file.
     * @param jsonID ID of the JSON configuration file.
     * @param scene Reference to the scene where the projectile will exist.
     * @return std::unique_ptr<Projectile> Configured projectile object.
     * @throws std::runtime_error If configuration is invalid or required fields are missing.
     */
    static std::unique_ptr<Projectile> createFromConfigFile(
        const std::string& jsonID, Scene& scene);

    /**
     * @brief Create a Projectile from JSON configuration.
     * @param scene Reference to the scene where the projectile will exist.
     * @param config JSON object containing projectile configuration.
     * @return std::unique_ptr<Projectile> Configured projectile object.
     * @throws std::runtime_error If configuration is invalid or required fields are missing.
     */
    static std::unique_ptr<Projectile> createFromJson(const nlohmann::json& config, Scene& scene);

private:
    /**
     * @brief Validate required fields in JSON configuration.
     * @param config JSON object to validate.
     * @throws std::runtime_error If required fields are missing or invalid.
     */
    static void validateConfig(const nlohmann::json& config);

    /**
     * @brief Parse targeting information from JSON.
     * @param targetingValue JSON value containing targeting information.
     * @return ProjectileTargetType The parsed targeting type.
     * @throws std::runtime_error If targeting type is invalid.
     */
    static int parseTargeting(const nlohmann::json& targetingValue);

    /**
     * @brief Create and configure flight mode based on configuration.
     * @param config JSON object containing projectile configuration.
     * @return FlightMode* Pointer to created flight mode (caller owns memory).
     * @throws std::runtime_error If flight mode type is unsupported.
     */
    static FlightMode* createFlightMode(const nlohmann::json& config);

    /**
     * @brief Parse and validate collision distance from JSON.
     * @param config JSON object containing collision distance.
     * @return float Validated collision distance value.
     */
    static float parseCollisionDistance(const nlohmann::json& config);

    /**
     * @brief Parse texture configuration from JSON and load sprite animation.
     * @param config JSON object containing texture configuration.
     * @param projectile Projectile to configure with texture animation.
     * @note Uses SpriteAnimation to handle texture loading and animation.
     */
    static void parseTexture(const nlohmann::json& config, Projectile& projectile);
};
