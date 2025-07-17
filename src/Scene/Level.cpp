#include "Scene/Level.hpp"

#include <array>
#include <format>
#include <fstream>
#include <iostream>
#include <json.hpp>

#include "Base/Constants.hpp"
#include "Gameplay/Difficulty.hpp"
#include "Utility/logger.hpp"

Level::Level(SceneManager &sceneManager)
    : Scene(sceneManager), currentWave{0} {}

void Level::update() {
    entityManager.update();
    for (std::vector<EnemyGroupInfo> &currentWave : waveInfo) {
        for (EnemyGroupInfo &group : currentWave) {
            if (group.quantity == 0) continue;
            if (group.spawnDelayTimer > 0.f) {
                group.spawnDelayTimer -= GameConstants::TICK_INTERVAL;
                if (group.spawnDelayTimer < 0.f) {
                    group.internalDelayTimer -= std::abs(group.spawnDelayTimer);
                    group.spawnDelayTimer = 0.f;
                }
            } else {
                group.internalDelayTimer -= GameConstants::TICK_INTERVAL;
                while (group.internalDelayTimer <= 0 && group.quantity) {
                    // ! Placeholder. Put enemy factory here
                    entityManager.addEnemy(
                        factory->createEnemy(group.id, 0, group.laneID));
                    Logger::info(std::format("Spawning {}", group.id));
                    group.internalDelayTimer += group.internalDelay;
                    group.quantity--;
                }
                if (group.quantity == 0) group.internalDelayTimer = 0.f;
            }
        }
    }
}

void Level::draw(sf::RenderTarget &target, sf::RenderStates state) const {
    target.draw(map, state);
    entityManager.render(state);
}
void Level::loadFromJson(const std::string &pathToFile) {
    nlohmann::json jsonFile = nlohmann::json::parse(std::ifstream(pathToFile));
    loadFromJson(jsonFile);
}

void Level::loadFromJson(const nlohmann::json &jsonFile) {
    loadWaypoints(jsonFile);
    loadWaves(jsonFile);

    factory = std::make_unique<EnemyFactory>(map, *this);

    std::string difficulty = jsonFile["difficulty"].get<std::string>();
    if (difficulty == "easy") factory->setDifficulty(Difficulty::Easy);
    if (difficulty == "medium") factory->setDifficulty(Difficulty::Medium);
    if (difficulty == "hard") factory->setDifficulty(Difficulty::Hard);
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

void Level::loadWaves(const nlohmann::json &jsonFile) {
    using namespace nlohmann;
    auto waveConfiguration = jsonFile["wave"];
    if (waveConfiguration.is_array() == false) {
        Logger::error("Not an array");
        return;
    }

    for (auto waveIt = waveConfiguration.begin();
         waveIt != waveConfiguration.end(); ++waveIt) {
        std::vector<EnemyGroupInfo> enemyGroups;
        for (auto groupIt = waveIt->begin(); groupIt != waveIt->end();
             ++groupIt) {
            EnemyGroupInfo groupInfo;
            groupInfo.id = (*groupIt)["id"];
            groupInfo.quantity = (*groupIt)["quantity"];
            groupInfo.spawnDelay = (*groupIt)["spawnDelay"];
            groupInfo.internalDelay = (*groupIt)["internalDelay"];
            groupInfo.laneID = (*groupIt)["lane"];

            groupInfo.spawnDelayTimer = groupInfo.spawnDelay;
            groupInfo.internalDelayTimer = groupInfo.internalDelay;

            enemyGroups.push_back(groupInfo);
        }
        waveInfo.push_back(enemyGroups);
    }
}

void Level::onLoad() {
    // TODO: Register enemies and towers on left click, open side menu showing
    // stats
}

void Level::onUnload() {
    // TODO: Unregister enemies and towers on left click, close side menu
}

bool Level::isWaveFinished() {
    std::vector<EnemyGroupInfo> &wave = waveInfo[currentWave];
    for (EnemyGroupInfo &group : wave) {
        if (group.quantity != 0) return false;
    }
    return true;
}