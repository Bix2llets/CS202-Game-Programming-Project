#include "Scene/Level.hpp"

#include <array>
#include <format>
#include <fstream>
#include <iostream>
#include <json.hpp>

#include "Base/Constants.hpp"
#include "Core/InputManager.hpp"
#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "GUIComponents/EnemyPanel.hpp"
#include "Gameplay/Difficulty.hpp"
#include "Utility/logger.hpp"

Level::Level() : currentWave{0}, isRunning{true} {
    subscribeKeyboard(Key::Space, UserEvent::Press,
                      InputManager::getInstance().getKeyboardState());
    entityManager.subscribeKeyboard(
        Key::D, UserEvent::Press,
        InputManager::getInstance().getKeyboardState());
    entityManager.subscribeKeyboard(
        Key::F, UserEvent::Press,
        InputManager::getInstance().getKeyboardState());
}

void Level::update() {
    if (!isRunning) return;
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
    drawBackground(target, state);
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
        std::vector<sf::Vector2f> waypoints;
        for (auto pointsIt = (*path).begin(); pointsIt != (*path).end();
             pointsIt++) {
            std::array<float, 2> waypoint = *pointsIt;
            waypoints.push_back({waypoint[0], waypoint[1]});
        }
        map.loadWaypoints(waypoints);
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
            groupInfo.spawnDelay = (*groupIt)["spawn_delay"];
            groupInfo.internalDelay = (*groupIt)["internal_delay"];
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
    entityManager.subscribeMouse(Mouse::Left, UserEvent::Press,
                                 InputManager::getInstance().getMouseState());
}

void Level::onUnload() {
    // TODO: Unregister enemies and towers on left click, close side menu
    entityManager.unSubscribeMouse(Mouse::Left, UserEvent::Press,
                                   InputManager::getInstance().getMouseState());
    EnemyPanel::getInstance().clearEnemy();
}

bool Level::isWaveFinished() {
    std::vector<EnemyGroupInfo> &wave = waveInfo[currentWave];
    for (EnemyGroupInfo &group : wave) {
        if (group.quantity != 0) return false;
    }
    return true;
}

void Level::onKeyEvent(Key key, UserEvent event,
                       const sf::Vector2f &worldPosition,
                       const sf::Vector2f &windowPosition) {
    if (key == Key::Space && event == UserEvent::Press) {
        isRunning = !isRunning;
    }
}

void Level::drawBackground(sf::RenderTarget &target,
                           sf::RenderStates state) const {
    const sf::Texture& gressTexture =
        *ResourceManager::getInstance().getTexture("grass");
    int textureWidth = gressTexture.getSize().x;
    int textureHeight = gressTexture.getSize().y;

    static const int width =  GameConstants::DEFAULT_WINDOW_WIDTH;
    static const int height = GameConstants::DEFAULT_WINDOW_HEIGHT;
    static const int tileWidth = 32;
    static const int tileHeight = 32;

    static const int horizontalTiles = textureWidth / tileWidth;
    static const int verticalTiles = textureHeight / tileHeight;

    auto hashGen = [](int val, int MOD) {
        return int(1LL * val * 22071997 % 101 % MOD);
    };
    for (int i = 0; i < width; i += tileWidth)
        for (int j = 0; j < height; j += tileHeight) {
            sf::Vector2i texturePosition = {hashGen(i / 32, horizontalTiles),
                                            hashGen(j / 32, verticalTiles)};
            sf::Vector2i textureSize = {tileWidth, tileHeight};
            sf::Sprite sprite(gressTexture);
            sprite.setTextureRect({texturePosition * 32, textureSize});
            sprite.setPosition(static_cast<sf::Vector2f>(sf::Vector2i{i, j}));
            // Logger::debug(std::format("{} {}",
            //                           texturePosition.x,
            //                           texturePosition.y));
            target.draw(sprite);
            // target.draw(sprite, state);
        }
}