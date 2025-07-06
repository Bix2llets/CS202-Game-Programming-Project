#include "Gameplay/Map.hpp"

#include <memory>
#include <string>
#include <vector>
Map::Map(std::vector<Waypoint> source) {

    // * Placeholder for testing waypoints
    mapWaypoints.push_back(std::make_unique<std::vector<Waypoint>>(source));
}

Map::Map(std::string sourceFileDirectory) {
    // * Will load from another json file
}

std::vector<Waypoint>* const Map::getWaypoints(int pathNumber) {
    return mapWaypoints[pathNumber].get();
}

void Map::draw(sf::RenderWindow& window, sf::RenderStates state) const {}