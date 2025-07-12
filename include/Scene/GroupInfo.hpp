#pragma once
#include <string>

/**
 * @struct GroupInfo
 * @brief Represents information about a group of enemies to be spawned in the game.
 *
 * This structure holds the configuration for spawning a group of enemies, including
 * the type of enemy, the number to spawn, and timing details for both group and individual spawns.
 *
 * @var std::string enemyType
 *      The type or identifier of the enemy to spawn.
 * @var int quantity
 *      The total number of enemies to spawn in this group.
 * @var float spawnDelay
 *      The delay (in seconds) before this group starts spawning.
 * @var float internalDelay
 *      The delay (in seconds) between spawning individual enemies within the group.
 * @var float spawnDelayTimer
 *      Timer tracking the elapsed time for the group's spawn delay.
 * @var float internalDelayTimer
 *      Timer tracking the elapsed time for the internal delay between enemy spawns.
 * @var int laneID
 *      The ID of the lane that the group spawns at.
 */
struct EnemyGroupInfo {
    std::string id;
    int quantity;
    float spawnDelay;
    float internalDelay;

    float spawnDelayTimer;
    float internalDelayTimer;

    int laneID;
};