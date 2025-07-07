/**
 * @file Waypoint.hpp
 * @brief Declares the Waypoint class for path navigation in the game.
 *
 * The Waypoint class represents a single point in a path, including its position and speed modifier.
 */
#pragma once

#include <SFML/System/Vector2.hpp>

/**
 * @class Waypoint
 * @brief Represents a single waypoint in a path, with position and speed multiplier.
 *
 * Waypoints are used by enemies and other entities to follow paths. Each waypoint has a position and an optional speed multiplier
 * that can be used to slow down or speed up entities as they pass through.
 */
class Waypoint {
public:
    /**
     * @brief Position of the waypoint in world coordinates.
     */
    const sf::Vector2f position;

    /**
     * @brief Speed multiplier at this waypoint (e.g., for slow/boost zones).
     */
    const float speedMultiplier;

    /**
     * @brief Construct a new Waypoint object
     * @param position The position of the waypoint
     * @param speedMultiplier The speed multiplier at this waypoint
     */
    Waypoint(sf::Vector2f position, float speedMultiplier)
        : position{position}, speedMultiplier{speedMultiplier} {}
};
