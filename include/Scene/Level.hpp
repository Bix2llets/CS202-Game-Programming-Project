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

#include "Core/KeyboardObserver.hpp"
#include "Entity/Factory/EnemyFactory.hpp"
#include "EntityManager.hpp"
#include "Gameplay/Weather/WeatherManager.hpp"
#include "Gameplay/Currency.hpp"
#include "Gameplay/RadialUpgradeMenu.hpp"
#include "Gameplay/Terrain/Path.hpp"
#include "Gameplay/Terrain/Terrain.hpp"
#include "Gameplay/TowerInfoPanel.hpp"
#include "Gameplay/TowerMenu.hpp"
#include "Gameplay/Tracker.hpp"
#include "Gameplay/Waypoint.hpp"
#include "Gameplay/Difficulty.hpp"

#include "Scene/GroupInfo.hpp"
#include "Scene/Overlays/Overlay.hpp"
#include "Scene/Scene.hpp"
#include "Scene/WaveManager.hpp"
#include "Gameplay/Randomness/RandomManager.hpp"
#include "GUIComponents/StaticSellMenu.hpp" // Include for StaticSellMenu


/**
 * @class Level
 * @brief Scene representing a gameplay level, with map, entities, and wave
 * logic.
 *
 * The Level scene manages the game map, all entities (enemies, towers,
 * projectiles), and wave progression. It supports loading from JSON, updating
 * game logic, and rendering.
 */
class Level : public Scene, public KeyboardObserver, public MouseObserver {
private:
    bool running;
    std::string levelID;

    // Gameplay Related
    DifficultyLevel difficulty;
    Health health;
    Currency budget;
    RandomManager randomManager;
    EntityManager entityManager;
    WeatherManager weatherManager;
    
    bool renderPath = false;
    Path path;
    
    Tracker tracker;
    
    // Wave Management
    int currentWave;
    int totalWaves;
    std::unique_ptr<EnemyFactory> factory;
    WaveManager waveManager;
    // Terrain map;  // game map for this\ level

    // GUI Related
    RadialUpgradeMenu upgradeMenu;
    StaticSellMenu staticSellMenu; // changed from StaticSellMenu staticSellMenu;
    TowerMenu menu;
    TowerInfoPanel infoPanel;
    std::unique_ptr<Overlay> overlay;
    sf::Sprite backgrounds;
    
    std::unique_ptr<RectangularButton> pauseButton;
    std::unique_ptr<RectangularButton> nextWaveButton;
    
public:
    Level();

    ~Level();
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
     * @brief Gets the tracker for this level.
     * @return Reference to the level's tracker.
     */
    Tracker &getTracker() { return tracker; }

    /**
     * @brief Gets the tracker for this level (const version).
     * @return Const reference to the level's tracker.
     */
    const Tracker &getTracker() const { return tracker; }

    /**
     * @brief Gets the random manager for this level.
     * @return Reference to the level's random manager.
     */
    RandomManager &getRandomManager() { return randomManager; }

    /**
     * @brief Get the Random Manager object
     * @return const RandomManager& 
     */
    const RandomManager &getRandomManager() const { return randomManager; }

    /**
     * @brief Get the Random object
     * @param type 
     * @return Random& 
     */
    Random& getRandom(RandomType type) {
        return randomManager.get(type);
    }

    /**
     * @brief Gets the entity manager for this level.
     * @return Reference to the level's entity manager.
     */
    inline EntityManager &getEntityManager() { return entityManager; };

    /**
     * @brief Gets the weather manager for this level.
     * @return Reference to the level's weather manager.
     */
    inline WeatherManager& getWeatherManager() { return weatherManager; }

    inline DifficultyLevel& getDifficulty() { return difficulty; }

    /**
     * @brief Gets the weather manager for this level (const version).
     * @return Const reference to the level's weather manager.
     */
    inline const WeatherManager& getWeatherManager() const { return weatherManager; }

    inline Currency getBudget() const { return budget; }
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
    void nextWave();

    /**
     * @brief Checks if the level is finished (last wave completed).
     * @return True if the level is finished, false otherwise.
     */
    inline bool isFinished() { return currentWave == totalWaves; }

    inline int getRemainingHealth() const { return health.getHealth(); }

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
    void subscribeCallbacks();

    public:
    bool onKeyEvent(Key key, UserEvent event, const sf::Vector2f &worldPosition,
                    const sf::Vector2f &windowPosition);
    bool onMouseEvent(Mouse mouse, UserEvent event,
                      const sf::Vector2f &worldPosition,
                      const sf::Vector2f &windowPosition) override;

    bool onScrollEvent(float delta, const sf::Vector2f &worldPosition,
                       const sf::Vector2f &windowPosition) override;

    inline bool isRunning() { return running; }

    inline sf::Vector2f getMapSize() const { return backgrounds.getGlobalBounds().size; }

    private:
    bool isPlacementValid(sf::Vector2f worldPosition);
};