#include "Scene/Level.hpp"

#include <array>
#include <format>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <memory>

#include "Base/Constants.hpp"
#include "Core/InputManager.hpp"
#include "Core/JSONLoader.hpp"
#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "Core/Window.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Factory/TowerFactory.hpp"
#include "GUIComponents/EnemyPanel.hpp"
#include "GUIComponents/cursor.hpp"
#include "Gameplay/Difficulty.hpp"
#include "Gameplay/TerrainParameter.hpp"
#include "Utility/logger.hpp"

#include "Entity/Factory/TowerFactory.hpp" // For testing purposes

Level::Level(TerrainParameter parameter, sf::Vector2f startingPoint,
             sf::Vector2f endPoint)
    : currentWave{0},
      isRunning{true},
      map(parameter),
      entityManager{map, *this},
      menu{budget, *this},
      tracker(*this) {
    subscribeKeyboard(Key::Space, UserEvent::Press,
                      InputManager::getInstance().getKeyboardState());
    subscribeKeyboard(Key::G, UserEvent::Press,
                      InputManager::getInstance().getKeyboardState());
    entityManager.subscribeKeyboard(
        Key::D, UserEvent::Press,
        InputManager::getInstance().getKeyboardState());
    entityManager.subscribeKeyboard(
        Key::F, UserEvent::Press,
        InputManager::getInstance().getKeyboardState());

    MouseState &mouseState = InputManager::getInstance().getMouseState();
    subscribeMouse(Mouse::Left, UserEvent::Press, mouseState);
    subscribeMouse(Mouse::Left, UserEvent::Release, mouseState);
    subscribeMouse(Mouse::Left, UserEvent::Move, mouseState);
    subscribeMouse(Mouse::None, UserEvent::Move, mouseState);
    subscribe("add_petrol", [this](std::any sender, std::any data) {
        try {
            int petrolAmount = std::any_cast<int>(data);
            budget.addPetroleum(petrolAmount);

        } catch (const std::bad_any_cast &e) {
            Logger::error("Sent illegal signal on add petrol");
        }
    });
    subscribe("add_scrap", [this](std::any sender, std::any data) {
        try {
            int scrapAmount = std::any_cast<int>(data);
            budget.addScraps(scrapAmount);

        } catch (const std::bad_any_cast &e) {
            Logger::error("Sent illegal signal on add scrap");
        }
    });
    subscribe("place_tower_cursor", [this](std::any sender, std::any data) {
        sf::Vector2f worldPosition = std::any_cast<sf::Vector2f>(data);

        TowerFactory factory;
        std::unique_ptr<Tower> newTower =
            std::move(factory.createFromConfigFile(
                Cursor::getInstance().getCarryingTowerID(), *this,
                worldPosition));

        entityManager.addTower(std::move(newTower));
    });
}

Level::~Level() {
    MouseState &mouseState = InputManager::getInstance().getMouseState();
    unSubscribeMouse(Mouse::Left, UserEvent::Press, mouseState);
    unSubscribeMouse(Mouse::Left, UserEvent::Release, mouseState);
    unSubscribeMouse(Mouse::Left, UserEvent::Move, mouseState);
    unSubscribeMouse(Mouse::None, UserEvent::Move, mouseState);
}
void Level::update() {
    menu.update();
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
                    entityManager.addEnemy(factory->createEnemy(group.id, 0));
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
    // drawBackground(target, state);
    Window::getInstance().toggleUserMode();
    map.render(state);
    entityManager.render(state);

    Window::getInstance().toggleGUIMode();
    menu.render(state);
}
void Level::loadFromJson(const std::string &pathToFile) {
    nlohmann::json jsonFile = nlohmann::json::parse(std::ifstream(pathToFile));
    loadFromJson(jsonFile);
}

void Level::loadFromJson(const nlohmann::json &jsonFile) {
    loadWaves(jsonFile);

    factory = std::make_unique<EnemyFactory>(map, *this);
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

bool Level::onKeyEvent(Key key, UserEvent event,
                       const sf::Vector2f &worldPosition,
                       const sf::Vector2f &windowPosition) {
    if (key == Key::Space && event == UserEvent::Press) {
        isRunning = !isRunning;
        return true;
    }

    if (key == Key::G && event == UserEvent::Press) {
        notify("add_petrol", 0, 10);
        notify("add_scrap", 0, 10);
        return true;
    }
    return false;
}

bool Level::onMouseEvent(Mouse mouse, UserEvent event,
                         const sf::Vector2f &worldPosition,
                         const sf::Vector2f &windowPosition) {
    if (menu.onMouseEvent(mouse, event, worldPosition, windowPosition)) {
        return true;
    }

    if (Cursor::getInstance().isDisplaying()) {
        if (isPlacementValid(worldPosition)) {
            Cursor::getInstance().setValidPlacement();
        } else {
            Cursor::getInstance().setInvalidPlacement();
        }
    }
    if (entityManager.onMouseEvent(mouse, event, worldPosition,
                                   windowPosition)) {
        return true;
    }

    return false;
}

bool Level::onScrollEvent(float delta, const sf::Vector2f &worldPosition,
                          const sf::Vector2f &windowPosition) {
    return false;
}

bool Level::isPlacementValid(sf::Vector2f worldPosition) {
    auto pointInQuad = [](const sf::Vector2f &pt,
                          const sf::Vector2f quad[4]) -> bool {
        auto sign = [](const sf::Vector2f &p1, const sf::Vector2f &p2,
                       const sf::Vector2f &p3) {
            return (p1.x - p3.x) * (p2.y - p3.y) -
                   (p2.x - p3.x) * (p1.y - p3.y);
        };
        bool b1, b2, b3, b4;
        b1 = sign(pt, quad[0], quad[1]) < 0.0f;
        b2 = sign(pt, quad[1], quad[2]) < 0.0f;
        b3 = sign(pt, quad[2], quad[3]) < 0.0f;
        b4 = sign(pt, quad[3], quad[0]) < 0.0f;
        if ((b1 == b2) && (b2 == b3) && (b3 == b4))
            Logger::debug(std::format("{} {} {} {}", b1, b2, b3, b4));
        return ((b1 == b2) && (b2 == b3) && (b3 == b4));
    };
    bool isValid = true;
    for (auto &tower : entityManager.getTowers())
        if (tower->contains(worldPosition)) return false;
    std::vector<Waypoint> pathway = *map.getPath();
    std::string towerID = Cursor::getInstance().getCarryingTowerID();

    int towerBaseWidth =
        JSONLoader::getInstance().getTower(towerID)["texture"]["width"];
    int towerBaseHeight =
        JSONLoader::getInstance().getTower(towerID)["texture"]["height"];
    for (int i = 0; i < pathway.size() - 1; i++) {
        Waypoint current = pathway[i];
        Waypoint nextPoint = pathway[i + 1];

        sf::Vector2f pathVector = pathway[i + 1].position - pathway[i].position;
        sf::Vector2f normal = {pathVector.y, -pathVector.x};
        normal = normal.normalized();
        sf::Vector2f pathRect[4] = {
            pathway[i].position + GameConstants::PATH_THICKNESS / 2.f * normal,
            pathway[i].position - GameConstants::PATH_THICKNESS / 2.F * normal,
            pathway[i + 1].position +
                GameConstants::PATH_THICKNESS / 2.f * normal,
            pathway[i + 1].position -
                GameConstants::PATH_THICKNESS / 2.f * normal};
        sf::Vector2f baseCornerPos[4] = {
            worldPosition + sf::Vector2f(towerBaseWidth, towerBaseHeight) / 2.f,
            worldPosition +
                sf::Vector2f(-towerBaseWidth, towerBaseHeight) / 2.f,
            worldPosition +
                sf::Vector2f(towerBaseWidth, -towerBaseHeight) / 2.f,
            worldPosition +
                sf::Vector2f(-towerBaseWidth, -towerBaseHeight) / 2.f,
        };

        // Helper lambda to check if a point is inside a convex quad

        // Check if any baseCornerPos is inside pathRect
        for (int j = 0; j < 4; ++j) {
            if (pointInQuad(baseCornerPos[j], pathRect)) {
                return false;
            }
        }
        // Check if any pathRect corner is inside baseCornerPos quad
        for (int j = 0; j < 4; ++j) {
            if (pointInQuad(pathRect[j], baseCornerPos)) {
                return false;
            }
        }
    }
    return true;
}