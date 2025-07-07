
/**
 * @file Map.hpp
 * @brief Declares the Map class for managing paths and waypoints in the game.
 *
 * The Map class manages collections of waypoints for different paths and provides drawing and access methods.
 */

#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "Gameplay/Waypoint.hpp"

/**
 * @class Map
 * @brief Manages collections of waypoints for different paths and provides drawing and access methods.
 *
 * The Map class stores multiple paths, each as a vector of Waypoints, and allows access to them by path number.
 * It also provides methods to construct a map from a vector of waypoints or from a file, and to draw the map.
 */
class Map : public sf::Drawable {
    std::vector<std::unique_ptr<std::vector<Waypoint>>> mapWaypoints; ///< Paths of waypoints in the map

public:
    /**
     * @brief Default constructor
     */
    Map() = default;

    /**
     * @brief Construct a map from a vector of waypoints
     * @param source Vector of waypoints representing a path
     */
    Map(std::vector<Waypoint> source);

    /**
     * @brief Construct a map from a file
     * @param sourceFileDirectory Path to the file containing map data
     */
    Map(std::string sourceFileDirectory);

    /**
     * @brief Get the waypoints for a specific path
     * @param pathNumber The index of the path
     * @return Pointer to the vector of waypoints for the path
     */
    std::vector<Waypoint>* const getWaypoints(int pathNumber);

    /**
     * @brief Draw the map (all waypoints/paths) to the render target
     * @param window The render window
     * @param state The render states
     */
    void draw(sf::RenderWindow& window, sf::RenderStates state) const;
};