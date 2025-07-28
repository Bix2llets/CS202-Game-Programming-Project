/**
 * @file Path.hpp
 * @brief Declares the Path class for managing paths and waypoints in the game.
 *
 * The Path class manages collections of waypoints for different paths and
 * provides drawing and access methods.
 */

#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

#include "Gameplay/Waypoint.hpp"

/**
 * @class Path
 * @brief Manages collections of waypoints for different paths and provides
 * drawing and access methods.
 *
 * The Path class stores multiple paths, each as a vector of sf::Vector2fs, and
 * allows access to them by path number. It also provides methods to construct a
 * map from a vector of waypoints or from a file, and to draw the map.
 */
class Path : public sf::Drawable {
    std::vector<Waypoint> waypoints;  ///< Paths of waypoints in the map

   public:
    /**
     * @brief Default constructor
     */
    Path() = default;
    /**
     * @brief Get the waypoints for a specific path
     * @param pathNumber The index of the path
     * @return Pointer to the vector of waypoints for the path
     */
    const std::vector<Waypoint>& getWaypoints();

    /**
     * @brief Draw the map (all waypoints/paths) to the render target
     * @param window The render window
     * @param state The render states
     */
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

    /** @brief Load the middle path from a vector of waypoints, then expand it
     * to parallel paths
     * @param waypoints The vector of waypoints of the middle path
     * @param pathID The ID of the path
     */
    void loadWaypoints(const std::vector<Waypoint>& path);

    // Move constructor
    Path(Path&& other) noexcept
        : waypoints(std::move(other.waypoints)),
          pathSprite(std::move(other.pathSprite)),
          pathTexture(std::move(other.pathTexture)) {}

    // Move assignment operator
    Path& operator=(Path&& other) noexcept {
        if (this != &other) {
            waypoints = std::move(other.waypoints);
            pathSprite = std::move(other.pathSprite);
            pathTexture = std::move(other.pathTexture);
        }
        return *this;
    }

   private:
    sf::RenderTexture getPathTexture() const;
    sf::RenderTexture getMaskTexture() const;
    std::unique_ptr<sf::Sprite> pathSprite;
    std::unique_ptr<sf::Texture> pathTexture;
};