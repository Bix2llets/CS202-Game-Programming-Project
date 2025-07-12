#include "Gameplay/Map.hpp"

#include <memory>
#include <string>
#include <vector>
#include <cmath>
const std::vector<Waypoint>* Map::getWaypoints(int pathNumber) {
    return &mapWaypoints[pathNumber];
}

void Map::draw(sf::RenderTarget& target,  sf::RenderStates state) const {
    for (const std::vector<Waypoint> path : mapWaypoints) {
        sf::VertexArray pathway(sf::PrimitiveType::TriangleStrip,
                                static_cast<int>(path.size() * 4 - 4));
        for (int i = 0; i < path.size(); i++) {
            if (i < path.size() - 1) {
                // Calculate direction vector
                sf::Vector2f dir = path[i + 1].position - path[i].position;
                float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                if (length != 0) dir /= length;

                // Perpendicular vector for thickness
                sf::Vector2f perp(-dir.y, dir.x);

                float thickness = 45.f;  // Set your desired thickness here

                // Offset points
                sf::Vector2f offset = (perp * (thickness / 2.f));
                if (i == 0) {
                    pathway[i * 4].position = path[i].position + offset - (dir * thickness) / 2.f;
                    pathway[i * 4 + 1].position = path[i].position - offset - (dir * thickness) / 2.f;

                }
                else {

                    pathway[i * 4].position = path[i].position + offset;
                    pathway[i * 4 + 1].position = path[i].position - offset;
                }

                if (i == path.size() - 2) {

                    pathway[i * 4 + 2].position = path[i + 1].position + offset + (dir * thickness) / 2.f;
                    pathway[i * 4 + 3].position = path[i + 1].position - offset + (dir * thickness) / 2.f;
                }
                else {
                    pathway[i * 4 + 2].position = path[i + 1].position + offset;
                    pathway[i * 4 + 3].position = path[i + 1].position - offset;

                }
            } else {
                // For the last point, repeat the previous offset
                // pathway[i * 2].position = (path[i].position - path[i - 1].position) + pathway[(i - 1) * 2].position;
                // pathway[i * 2 + 1].position = (path[i].position - path[i - 1].position) + pathway[(i - 1) * 2 + 1].position;
            }
        }
        for (int i = 0; i < pathway.getVertexCount(); i++)
            pathway[i].color = sf::Color::White;
        
        if (path.size() > 1) target.draw(pathway, state);
    }
}

void Map::loadWaypoints(const std::vector<Waypoint>& path, int pathID) {
    if (mapWaypoints.size() <
        pathID + 1)  //> Accomodate for the starting  index of 0
        mapWaypoints.resize(pathID + 1);

    mapWaypoints[pathID] = path;
}