/**
 * @file AreaEffectFactory.hpp
 * @brief Declares the AreaEffectFactory for creating AreaEffect entities from JSON.
 */
#pragma once

#include <json.hpp>
#include <memory>
#include <string>

// Forward declarations
class AreaEffect;
class Scene;

/**
 * @class AreaEffectFactory
 * @brief Factory for creating AreaEffect entities from config JSON.
 */
class AreaEffectFactory {
public:
	/**
	 * @brief Create an AreaEffect from a configuration file entry (by ID).
	 * @param jsonID ID of the area-effect configuration.
	 * @param scene Scene where the entity will exist.
	 * @return std::unique_ptr<AreaEffect>
	 */
	static std::unique_ptr<AreaEffect> createFromConfigFile(
		const std::string& jsonID, Scene& scene, uint64_t sourceId);

	/**
	 * @brief Create an AreaEffect directly from JSON configuration.
	 * @param config JSON object containing area-effect configuration.
	 * @param scene Scene where the entity will exist.
	 * @return std::unique_ptr<AreaEffect>
	 */
	static std::unique_ptr<AreaEffect> createFromJson(
		const nlohmann::json& config, Scene& scene, uint64_t sourceId);

private:
	static void validateConfig(const nlohmann::json& config);
	static void parseTexture(const nlohmann::json& config, AreaEffect& effect);
	static int parseDamageType(const nlohmann::json& value);
};

