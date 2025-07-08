#include "Scene/Level.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <json.hpp>
Level::Level(sf::RenderWindow &window, const std::string &name,
             SceneManager &sceneManager, InputManager &inputManager,
             ResourceManager &resourceManager)
    : Scene(window, name, sceneManager, inputManager, resourceManager) {}

void Level::update() {}

void Level::draw(sf::RenderTarget &target, sf::RenderStates state) const {
    target.draw(map, state);
}
void Level::loadFromJson(const std::string &pathToFile) {
    nlohmann::json jsonFile = nlohmann::json::parse(std::ifstream(pathToFile));
    loadFromJson(jsonFile);
}

void Level::loadFromJson(const nlohmann::json &jsonFile) {
    loadWaypoints(jsonFile);
}

void Level::loadWaypoints(const nlohmann::json &jsonFile) {
    using namespace nlohmann;
    auto waypointsData = jsonFile["waypoints"];
    int pathNumber = 0;
    for (auto path = waypointsData.begin(); path != waypointsData.end();
         path++, pathNumber++) {
        std::vector<Waypoint> waypoints;
        for (auto pointsIt = (*path).begin(); pointsIt != (*path).end();
             pointsIt++) {
            std::array<float, 3> waypoint = *pointsIt;
            waypoints.push_back(
                Waypoint({waypoint[0], waypoint[1]}, waypoint[2]));
        }
        map.loadWaypoints(waypoints, pathNumber);
    }
}

void Level::registerComponents() {
    // TODO: Register enemies and towers on left click, open side menu showing stats
}

void Level::unRegisterComponents() {
    // TODO: Unregister enemies and towers on left click, close side menu
}