/**
 * @file StaticEntityFactory.hpp
 * @brief Declares the StaticEntityFactory for creating StaticEntity entities from JSON.
 */
#pragma once

#include <json.hpp>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

// Forward declarations
class StaticEntity;
class Scene;

/**
 * @class StaticEntityFactory
 * @brief Factory for creating StaticEntity entities from config JSON.
 */
class StaticEntityFactory {
public:
	/**
	 * @brief Create a StaticEntity from a configuration file entry (by ID).
	 * @param jsonID ID of the static-entity configuration.
	 * @param scene Scene where the entity will exist.
	 * @return std::unique_ptr<StaticEntity>
	 */
	static std::unique_ptr<StaticEntity> createFromConfigFile(
		const std::string& jsonID, Scene& scene, const sf::Vector2f& position = sf::Vector2f(0, 0));

	/**
	 * @brief Create a StaticEntity directly from JSON configuration.
	 * @param config JSON object containing static-entity configuration.
	 * @param scene Scene where the entity will exist.
	 * @return std::unique_ptr<StaticEntity>
	 */
	static std::unique_ptr<StaticEntity> createFromJson(
		const nlohmann::json& config, Scene& scene, const sf::Vector2f& position = sf::Vector2f(0, 0));

private:
	static void validateConfig(const nlohmann::json& config);
	static void parseTexture(const nlohmann::json& config, StaticEntity& entity);
	static void parseCurrency(const nlohmann::json& currencyJson, int& scrap, int& petroleum);
};
