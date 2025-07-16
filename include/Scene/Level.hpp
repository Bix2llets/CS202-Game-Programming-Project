#pragma once
/**
 * @file Level.hpp
 * @brief Declares the Level scene class for gameplay levels.
 *
 * The Level class implements a gameplay scene, managing the map, entities, wave
 * progression, and integration with the scene management system. It provides
 * methods for loading level data from JSON, updating game logic, and rendering
 * the level.
 */

#include <json.hpp>

#include "Core/JSONLoader.hpp"
#include "Entity/Factory/EnemyFactory.hpp"
#include "EntityManager.hpp"
#include "Gameplay/Map.hpp"
#include "Gameplay/Waypoint.hpp"
#include "Scene/GroupInfo.hpp"
#include "Scene/Scene.hpp"
class SceneManager;
class InputManager;
class ResourceManager;

/**
 * @class Level
 * @brief Scene representing a gameplay level, with map, entities, and wave
 * logic.
 *
 * The Level scene manages the game map, all entities (enemies, towers,
 * projectiles), and wave progression. It supports loading from JSON, updating
 * game logic, and rendering.
 */
class Level : public Scene {
   private:
    std::string levelID;  ///< Unique identifier for the level
    void loadLevelID(const nlohmann::json &jsonfile);
    EntityManager entityManager;  ///< Manages all entities in the level
    std::unique_ptr<EnemyFactory> factory;
    Map map;  ///< The game map for this level
    std::vector<std::vector<EnemyGroupInfo>>
        waveInfo;     ///< Information for each wave
    int currentWave;  ///< Index of the current wave

   public:
    /**
     * @brief Constructs the Level scene.
     * @param window Reference to the SFML render window.
     * @param parentManager Reference to the parent SceneManager.
     * @param resourceManager Reference to the ResourceManager.
     * @param JSONLoader Reference to the JSON loader.
     */
    Level(sf::RenderWindow &window, SceneManager &parentManager,
          InputManager &inputManager, ResourceManager &resourceManager,
          JSONLoader &loader);

    /**
     * @brief Updates the level logic (entities, waves, etc).
     */
    void update();

    /**
     * @brief Draws the level and its entities to the target.
     * @param target The render target to draw to.
     * @param state Current render states.
     */
    void draw(sf::RenderTarget &target, sf::RenderStates state) const;

    /**
     * @brief Loads level data from a JSON file path.
     * @param pathToFile Path to the JSON file.
     */
    void loadFromJson(const std::string &pathToFile);

    /**
     * @brief Loads level data from a JSON object.
     * @param jsonFile The JSON object containing level data.
     */
    void loadFromJson(const nlohmann::json &jsonFile);

    /**
     * @brief Gets the unique ID of the level.
     * @return The level's unique ID string.
     */
    inline const std::string getID() const { return "Level" + levelID; }

    /**
     * @brief Registers UI components and event handlers for the level.
     */
    void onLoad() override;

    /**
     * @brief Unregisters UI components and event handlers for the level.
     */
    void onUnload() override;

    /**
     * @brief Checks if the current wave is finished.
     * @return True if the wave is finished, false otherwise.
     */
    bool isWaveFinished();

    /**
     * @brief Advances to the next wave if available.
     */
    inline void nextWave() {
        if (currentWave < waveInfo.size() - 1) ++currentWave;
    }

    /**
     * @brief Checks if the level is finished (last wave completed).
     * @return True if the level is finished, false otherwise.
     */
    inline bool isFinished() { return currentWave == waveInfo.size() - 1; }

   private:
    /**
     * @brief Loads waypoints from the provided JSON file.
     * @param jsonFile The JSON object containing waypoint data.
     */
    void loadWaypoints(const nlohmann::json &jsonFile);
    /**
     * @brief Loads wave information from the provided JSON file.
     * @param jsonFile The JSON object containing wave data.
     */
    void loadWaves(const nlohmann::json &jsonFile);
    /**
     * @brief Loads the level ID from the provided JSON file.
     * @param jsonfile The JSON object containing the level ID.
     */
};