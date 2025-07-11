#pragma once
/**
 * @file JSONLoader.hpp
 * @brief Declares the JSONLoader class for loading and retrieving game data from JSON files.
 *
 * The JSONLoader class is responsible for loading all JSON files from the `mod` and `content` folders,
 * specifically from their `enemy`, `level`, `projectile`, and `tower` subfolders. It supports loading
 * multiple JSON files per type, each of which may contain a single object or an array of objects. The loader
 * provides efficient retrieval of JSON objects by type and ID, ensuring that objects with the same ID but
 * different types (e.g., enemy vs. tower) are not confused.
 *
 * Usage:
 *   - Call loadAll() to load all JSON data from the relevant directories.
 *   - Use getEnemy(), getLevel(), getProjectile(), or getTower() to retrieve a specific object by ID.
 *   - Use getAllEnemies(), getAllLevels(), getAllProjectiles(), or getAllTowers() to access all loaded objects of a type.
 */

#include <string>
#include <unordered_map>
#include <json.hpp>

/**
 * @class JSONLoader
 * @brief Loads and manages game data from JSON files in mod/content subfolders.
 *
 * This class loads all JSON files from the `enemy`, `level`, `projectile`, and `tower` subfolders
 * of both the `mod` and `content` directories (located at the same level as the executable). It supports
 * files containing either a single JSON object or an array of objects. Each object must have a unique ID
 * within its type. The loader provides type-safe retrieval methods to avoid ID collisions between types.
 */
class JSONLoader {
public:
    /**
     * @brief Loads all JSON files from mod and content subfolders (enemy, level, projectile, tower).
     *
     * This method scans the relevant directories, parses all JSON files, and stores the objects
     * in internal maps for fast retrieval. Supports both single-object and array-of-objects files.
     */
    void loadAll();

    /**
     * @brief Retrieve a JSON object by ID for the enemy type.
     * @param id The unique ID of the enemy object.
     * @return Reference to the JSON object if found, otherwise throws or returns a null reference.
     */
    const nlohmann::json& getEnemy(const std::string& id) const;

    /**
     * @brief Retrieve a JSON object by ID for the level type.
     * @param id The unique ID of the level object.
     * @return Reference to the JSON object if found, otherwise throws or returns a null reference.
     */
    const nlohmann::json& getLevel(const std::string& id) const;

    /**
     * @brief Retrieve a JSON object by ID for the projectile type.
     * @param id The unique ID of the projectile object.
     * @return Reference to the JSON object if found, otherwise throws or returns a null reference.
     */
    const nlohmann::json& getProjectile(const std::string& id) const;

    /**
     * @brief Retrieve a JSON object by ID for the tower type.
     * @param id The unique ID of the tower object.
     * @return Reference to the JSON object if found, otherwise throws or returns a null reference.
     */
    const nlohmann::json& getTower(const std::string& id) const;

    /**
     * @brief Retrieve all loaded enemy objects.
     * @return Const reference to the map of enemy ID to JSON object.
     */
    const std::unordered_map<std::string, nlohmann::json>& getAllEnemies() const;

    /**
     * @brief Retrieve all loaded level objects.
     * @return Const reference to the map of level ID to JSON object.
     */
    const std::unordered_map<std::string, nlohmann::json>& getAllLevels() const;

    /**
     * @brief Retrieve all loaded projectile objects.
     * @return Const reference to the map of projectile ID to JSON object.
     */
    const std::unordered_map<std::string, nlohmann::json>& getAllProjectiles() const;

    /**
     * @brief Retrieve all loaded tower objects.
     * @return Const reference to the map of tower ID to JSON object.
     */
    const std::unordered_map<std::string, nlohmann::json>& getAllTowers() const;

private:
    /**
     * @brief Helper to load all JSON files from a given directory and store them in the provided map.
     * @param directory The directory to scan for JSON files.
     * @param outMap The map to populate with ID to JSON object pairs.
     */
    void loadFromDirectory(const std::string& directory, std::unordered_map<std::string, nlohmann::json>& outMap);

    // Internal maps for each type: ID -> JSON object
    std::unordered_map<std::string, nlohmann::json> enemies;      ///< Enemy objects by ID
    std::unordered_map<std::string, nlohmann::json> levels;       ///< Level objects by ID
    std::unordered_map<std::string, nlohmann::json> projectiles;  ///< Projectile objects by ID
    std::unordered_map<std::string, nlohmann::json> towers;       ///< Tower objects by ID
};

