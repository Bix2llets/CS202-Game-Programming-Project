
/**
 * @file LevelFactory.hpp
 * @brief Declares the LevelFactory class for managing and instantiating game
 * levels from JSON configuration.
 *
 * LevelFactory loads, stores, and provides access to level configurations
 * defined in JSON format. It can return all configurations as a JSON array, and
 * instantiate levels by ID as unique pointers to Scene.
 *
 * - loadConfig: Loads a single scene configuration (must be a JSON object),
 * prints an error if not.
 * - getConfig: Returns a JSON array of all stored configurations (without their
 * names/IDs).
 * - getLevel: Returns a unique_ptr<Scene> for the level whose id matches the
 * given levelName.
 */

#pragma once

#include <SFML/Window.hpp>
#include <json.hpp>
#include <string>
#include <unordered_map>

#include "Scene/Level.hpp"
class SceneManager;
class InputManager;
class ResourceManager;

/**
 * @class LevelFactory
 * @brief Manages loading, storing, and instantiating game levels from JSON
 * configuration.
 */
class LevelFactory {
   private:
    /**
     * @brief Stores level configurations, mapped by their string IDs.
     */
    std::unordered_map<std::string, nlohmann::json> levelConfiguration;

    SceneManager &sceneManager;
    InputManager &inputManager;
    ResourceManager &resourceManager;

   public:
    /**
     * @brief Constructs a LevelFactory with references to core game systems.
     * @param window Reference to the main render window.
     * @param sceneManager Reference to the scene manager.
     * @param inputManager Reference to the input manager.
     * @param resourceManager Reference to the resource manager.
     * @param loader Reference to the JSON loader.
     */
    LevelFactory(SceneManager &sceneManager,
                 InputManager &inputManager, ResourceManager &resourceManager)
        : 
          sceneManager{sceneManager},
          inputManager{inputManager},
          resourceManager{resourceManager} {}

    /**
     * @brief Loads a single scene configuration from a JSON object.
     *        Prints an error if the input is not a JSON object.
     * @param config The JSON object representing the scene configuration.
     */
    void loadConfig(const nlohmann::json &config);

    /**
     * @brief Returns a JSON array of all stored level configurations (without
     * their names/IDs).
     * @return nlohmann::json array of all configurations.
     */
    nlohmann::json getConfig();

    /**
     * @brief Returns a unique_ptr to the Scene for the level whose id matches
     * levelName.
     * @param ID The id of the level to instantiate.
     * @return std::unique_ptr<Scene> for the requested level, or nullptr if not
     * found.
     */
    std::unique_ptr<Level> getLevel(const std::string &ID);
};