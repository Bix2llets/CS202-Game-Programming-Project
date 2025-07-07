/**
 * @file EnemyFactory.hpp
 * @brief Declares the EnemyFactory class for creating enemy entities.
 *
 * The EnemyFactory centralizes the creation of enemies, applying difficulty scaling and path assignment.
 */
#pragma once
#include <SFML/Graphics.hpp>

class Enemy;
enum class Difficulty;
class Map;
class Scene;

/**
 * @class EnemyFactory
 * @brief Factory for creating Enemy objects with difficulty scaling and path assignment.
 *
 * The EnemyFactory is responsible for instantiating enemies with the correct stats, position, and state based on game difficulty and map.
 */
class EnemyFactory {
private:
    static constexpr float REWARD_BASIC = 7; ///< Base reward for defeating a basic enemy
    Map &map; ///< Reference to the game map for waypoint assignment
    Scene &scene; ///< Reference to the current scene
    float rewardMultiplier; ///< Multiplier for enemy rewards based on difficulty
    float speedMultiplier; ///< Multiplier for enemy speed based on difficulty
    float healthMultiplier; ///< Multiplier for enemy health based on difficulty

public:
    /**
     * @brief Construct a new EnemyFactory
     * @param difficulty Game difficulty setting
     * @param map Reference to the game map
     * @param scene Reference to the current scene
     */
    EnemyFactory(Difficulty difficulty, Map &map, Scene &scene);

    /**
     * @brief Create a basic enemy with specified position, rotation, and lane
     * @param position Spawn position
     * @param angle Initial rotation
     * @param laneID Path/lane identifier
     * @return Enemy instance
     */
    Enemy createBasicEnemy(sf::Vector2f position, sf::Angle angle, int laneID);
    // * To be added more later
};