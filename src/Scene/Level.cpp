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
#include "Core/LevelFactory.hpp"
#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/SceneManager.hpp"
#include "Core/UserEvent.hpp"
#include "Core/Window.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Factory/TowerFactory.hpp"
#include "Entity/Factory/StaticEntityFactory.hpp"
#include "GUIComponents/EnemyPanel.hpp"
#include "GUIComponents/cursor.hpp"
#include "Gameplay/Difficulty.hpp"
#include "Gameplay/Terrain/TerrainParameter.hpp"
#include "Gameplay/TowerInfoPanel.hpp"
#include "Scene/Overlays/GameoverScreen.hpp"
#include "Scene/Overlays/PauseScreen.hpp"
#include "Utility/CollisionChecker.hpp"
#include "Utility/logger.hpp"

#include "Entity/Enemy/EnemySpawnInfo.hpp"
Level::Level()
    : currentWave{0},
      running{true},
      //   map(parameter),
      entityManager{*this},
      menu{budget, this},
      tracker(*this),
      upgradeMenu(*this),
      backgrounds(GameConstants::BLANK_TEXTURE),
      waveManager{*this},
      overlay{nullptr},
      randomManager() {
    health.setMaxHealth(200).setHealth(200);

    std::vector<std::unique_ptr<StaticEntity>> staticEntities;
    staticEntities.push_back(move(StaticEntityFactory::createFromConfigFile("rock_big_1", *this, sf::Vector2f(365, 310))));
    // staticEntities.push_back(move(StaticEntityFactory::createFromConfigFile("big_rock_2", *this, sf::Vector2f(100, 100))));

    for (auto& entity : staticEntities) {
        entityManager.addStaticEntity(std::move(entity));
    }

    MouseState &mouseState = InputManager::getInstance().getMouseState();
    subscribeCallbacks();
}

Level::~Level() {
    onUnload();
    overlay.reset(nullptr);
}
void Level::update() {
    if (overlay) {
        overlay->update();
        return;
    }

    menu.update();
    if (upgradeMenu.isDisplaying()) {
        upgradeMenu.update();
    }

    if (infoPanel.isDisplaying()) {
        infoPanel.update();
    }

    entityManager.update();
    waveManager.update();
}

void Level::draw(sf::RenderTarget &target, sf::RenderStates state) const {
    // drawBackground(target, state);
    Window::getInstance().toggleUserMode();
    // map.render(state);
    Window::getInstance().getRenderWindow().draw(backgrounds, state);
    entityManager.render(state);
    if (upgradeMenu.isDisplaying()) {
        upgradeMenu.render(state);
    }
    if (renderPath) path.draw(target, state);

    Window::getInstance().toggleGUIMode();
    menu.render(state);

    if (infoPanel.isDisplaying()) {
        infoPanel.render();
    }
    if (overlay) {
        overlay->render();
    }
}
void Level::loadFromJson(const std::string &pathToFile) {
    nlohmann::json jsonFile = nlohmann::json::parse(std::ifstream(pathToFile));
    loadFromJson(jsonFile);
}

void Level::loadFromJson(const nlohmann::json &jsonFile) {
    backgrounds = sf::Sprite(
        *ResourceManager::getInstance().getTexture(jsonFile["background"]));
    levelID = jsonFile["id"];
    sf::Vector2f scale;
    // scale.x = static_cast<float>(GameConstants::MAP_WIDTH) /
    //           backgrounds.getLocalBounds().size.x;
    // scale.y = static_cast<float>(GameConstants::MAP_HEIGHT) /
    //           backgrounds.getGlobalBounds().size.y;
    scale.x = jsonFile["size"]["x"].get<float>() /
              backgrounds.getLocalBounds().size.x;
    scale.y = jsonFile["size"]["y"].get<float>() /
              backgrounds.getLocalBounds().size.y;
    backgrounds.setScale(scale);
    std::vector<Waypoint> waypoints;
    for (const auto point : jsonFile["waypoints"]) {
        waypoints.emplace_back(
            Waypoint{sf::Vector2f{point[0].get<float>() * scale.x,
                                  point[1].get<float>() * scale.y},
                     1.0});
        // Logger::debug(std::format("Waypoint at {}, {}",
        // point[0].get<float>(),
        //                           point[1].get<float>()));
    }
    path.loadWaypoints(waypoints);
    loadWaves(jsonFile);
    Logger::success("Loaded waypoints");
    factory = std::make_unique<EnemyFactory>(waypoints, *this);
}

void Level::loadWaves(const nlohmann::json &jsonFile) {
    using namespace nlohmann;
    auto waveConfiguration = jsonFile["wave"];
    if (waveConfiguration.is_array() == false) {
        Logger::error("Not an array");
        return;
    }
    waveManager.loadJSON(jsonFile);
}

void Level::onLoad() {
    subscribeKeyboard(Key::Space, UserEvent::Press,
                      InputManager::getInstance().getKeyboardState());
    subscribeKeyboard(Key::G, UserEvent::Press,
                      InputManager::getInstance().getKeyboardState());

    subscribeKeyboard(Key::D, UserEvent::Press,
                      InputManager::getInstance().getKeyboardState());
    subscribeKeyboard(Key::F, UserEvent::Press,
                      InputManager::getInstance().getKeyboardState());
    subscribeKeyboard(Key::N, UserEvent::Press,
                      InputManager::getInstance().getKeyboardState());
    subscribeMouse(Mouse::Left, UserEvent::Press,
                   InputManager::getInstance().getMouseState());
    subscribeMouse(Mouse::Left, UserEvent::Release,
                   InputManager::getInstance().getMouseState());

    subscribeMouse(Mouse::Left, UserEvent::Move,
                   InputManager::getInstance().getMouseState());

    subscribeMouse(Mouse::None, UserEvent::Move,
                   InputManager::getInstance().getMouseState());
    subscribeMouse(Mouse::Right, UserEvent::Move,
                   InputManager::getInstance().getMouseState());
    subscribeMouse(Mouse::Middle, UserEvent::Move,
                   InputManager::getInstance().getMouseState());
    Window::getInstance().setLevelSize(
        sf::Vector2f{backgrounds.getGlobalBounds().size});
    Logger::debug(std::format("Level onLoad done on ", (void *)this));
}

void Level::onUnload() {
    unSubscribeKeyboard(Key::Space, UserEvent::Press,
                        InputManager::getInstance().getKeyboardState());
    unSubscribeKeyboard(Key::G, UserEvent::Press,
                        InputManager::getInstance().getKeyboardState());

    unSubscribeKeyboard(Key::D, UserEvent::Press,
                        InputManager::getInstance().getKeyboardState());
    unSubscribeKeyboard(Key::F, UserEvent::Press,
                        InputManager::getInstance().getKeyboardState());
    // TODO: Unregister enemies and towers on left click, close side menu
    EnemyPanel::getInstance().clearEnemy();
    Cursor::getInstance().clearCarryingTower();
    Cursor::getInstance().removeRenderImage();
    upgradeMenu.removeFocus();

    MouseState &mouseState = InputManager::getInstance().getMouseState();
    unSubscribeMouse(Mouse::Left, UserEvent::Press, mouseState);
    unSubscribeMouse(Mouse::Left, UserEvent::Release, mouseState);
    unSubscribeMouse(Mouse::Left, UserEvent::Move, mouseState);
    unSubscribeMouse(Mouse::None, UserEvent::Move, mouseState);
    unSubscribeMouse(Mouse::Right, UserEvent::Move,
                     InputManager::getInstance().getMouseState());
    unSubscribeMouse(Mouse::Middle, UserEvent::Move,
                     InputManager::getInstance().getMouseState());
    Logger::debug(std::format("Level onUnLoad done on ", (void *)this));
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
        if (!overlay) {
            overlay = std::make_unique<GameoverScreen>(*this);
            Cursor::getInstance().clearCarryingTower();
            Cursor::getInstance().removeRenderImage();
            EnemyPanel::getInstance().clearEnemy();
            upgradeMenu.removeFocus();
        } else {
            overlay = nullptr;
        }
        return true;
    }

    if (key == Key::G && event == UserEvent::Press) {
        notify("add_currency", 0, Currency(100, 100));
        return true;
    }
    if (key == Key::N && event == UserEvent::Press) {
        waveManager.nextWave();
        return true;
    }
    return false;
}

bool Level::onMouseEvent(Mouse mouse, UserEvent event,
                         const sf::Vector2f &worldPosition,
                         const sf::Vector2f &windowPosition) {
    if (overlay.get()) {
        return overlay->onMouseEvent(mouse, event, worldPosition,
                                     windowPosition);
    }

    if (!infoPanel.isDisplaying())
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

    if (upgradeMenu.onMouseEvent(mouse, event, worldPosition, windowPosition)) {
        return true;
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
    Window::getInstance().toggleUserMode();
    if (menu.contains((sf::Vector2f)Window::getInstance()
                          .getRenderWindow()
                          .mapCoordsToPixel(worldPosition)))
        return false;
    std::string towerID = Cursor::getInstance().getCarryingTowerID();

    nlohmann::json obj = JSONLoader::getInstance().getTower(towerID);

    int petroleumCost = obj["cost"]["petroleum"];
    int scrapCost = obj["cost"]["scrap"];

    bool isValid = true;
    int towerBaseWidth =
        JSONLoader::getInstance().getTower(towerID)["texture"]["width"];
    int towerBaseHeight =
        JSONLoader::getInstance().getTower(towerID)["texture"]["height"];

    sf::Vector2f upperLeft =
        worldPosition +
        sf::Vector2f{(float)-towerBaseWidth, (float)-towerBaseHeight} / 2.f;
    sf::Vector2f lowerLeft =
        worldPosition +
        sf::Vector2f{(float)-towerBaseWidth, (float)+towerBaseHeight} / 2.f;
    sf::Vector2f lowerRight =
        worldPosition +
        sf::Vector2f{(float)+towerBaseWidth, (float)+towerBaseHeight} / 2.f;
    sf::Vector2f upperRight =
        worldPosition +
        sf::Vector2f{(float)+towerBaseWidth, (float)-towerBaseHeight} / 2.f;
    sf::Vector2f towerBound[4] = {upperLeft, upperRight, lowerRight, lowerLeft};

    // for (float x = upperLeft.x; x <= lowerRight.x;
    //      x += GameConstants::CELL_SIZE)
    //     for (float y = upperLeft.y; y <= lowerRight.y;
    //          y += GameConstants::CELL_SIZE)
    //         if (map.getCellType({x, y}) == Height::DeepSea) return false;
    if (scrapCost > budget.getScraps().value ||
        petroleumCost > budget.getPetroleum().value)
        return false;
    for (auto &tower : entityManager.getTowers())
        if (tower->intersects(towerBound)) return false;
    const std::vector<Waypoint> &pathway = path.getWaypoints();

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
        if (CollisionChecker::isQuadilateralCrossed(pathRect, towerBound))
            return false;
    };
    return true;
}

void Level::subscribeCallbacks() {
    subscribe("add_currency", [this](std::any sender, std::any data) {
        try {
            Currency currency = std::any_cast<Currency>(data);
            budget += currency;
            Window::getInstance().toggleUserMode();

            if (Cursor::getInstance().isDisplaying())
                if (isPlacementValid(
                        Window::getInstance()
                            .getRenderWindow()
                            .mapPixelToCoords(static_cast<sf::Vector2i>(
                                Cursor::getInstance().getPosition())))) {
                    Cursor::getInstance().setValidPlacement();
                }

        } catch (const std::bad_any_cast &e) {
            Logger::error("Sent illegal signal on adding petroleum");
        }
    });
    subscribe("subtract_currency", [this](std::any sender, std::any data) {
        try {
            Currency currency = std::any_cast<Currency>(data);
            budget -= currency;
            Window::getInstance().toggleUserMode();
            if (Cursor::getInstance().isDisplaying())
                if (isPlacementValid(
                        Window::getInstance()
                            .getRenderWindow()
                            .mapPixelToCoords(static_cast<sf::Vector2i>(
                                Cursor::getInstance().getPosition())))) {
                    Cursor::getInstance().setValidPlacement();
                }

        } catch (const std::bad_any_cast &e) {
            Logger::error("Sent illegal signal on subtracting budget");
        }
    });
    subscribe("place_tower_cursor", [this](std::any sender, std::any data) {
        sf::Vector2f worldPosition = std::any_cast<sf::Vector2f>(data);

        if (isPlacementValid(worldPosition)) {
            std::unique_ptr<Tower> newTower =
                std::move(TowerFactory::createFromConfigFile(
                    Cursor::getInstance().getCarryingTowerID(), *this,
                    worldPosition));

            budget.subtractPetroleum(newTower->getCost().getPetroleum().value);
            budget.subtractScraps(newTower->getCost().getScraps().value);
            entityManager.addTower(std::move(newTower));
        }
    });

    subscribe("sell_tower", [this](std::any sender, std::any data) {
        try {
            Tower *tower = std::any_cast<Tower *>(sender);

            Currency amount = tower->getTotalCost();
            amount = amount * 0.6f;
            budget += amount;
            notify("unfocus_tower");
            entityManager.removeTower(tower);
        } catch (std::bad_any_cast &e) {
            Logger::error("Sent illegal signal on sell tower");
            return;
        }
    });

    subscribe("focus_tower", [this](std::any sender, std::any data) {
        try {
            Tower *tower = std::any_cast<Tower *>(sender);
            upgradeMenu.setFocus(tower);
            infoPanel.setFocus(tower);
        } catch (std::bad_any_cast &e) {
            Logger::error("Sent illegal signal on focus tower");
            return;
        }
    });

    subscribe("unfocus_tower", [this](std::any sender, std::any data) {
        upgradeMenu.removeFocus();
        infoPanel.deFocus();
    });

    subscribe("enemy_passed", [this](std::any sender, std::any data) {
        try {
            Enemy *enemySent = std::any_cast<Enemy *>(sender);
            health.takeDamage(enemySent->getHealth());
            if (health.getHealth() == 0) {
                menu.update();
                Logger::error("Level failed, health reached zero");
                overlay = std::make_unique<GameoverScreen>(*this);
            } else {
                Logger::warning(std::format("Enemy passed, health left: {}",
                                            health.getHealth()));
            }
        } catch (std::bad_any_cast &e) {
            Logger::error("Sent illegal signal on enemy passed");
            return;
        }
    });

    subscribe("show_upgrade_preview", [this](std::any sender, std::any data) {
        try {
            if (data.type() == typeid(std::nullptr_t)) {
                infoPanel.clearDisplayUpgrade();
                return;
            }
            const UpgradeDetails *upgradeDetail =
                std::any_cast<const UpgradeDetails *>(data);
            infoPanel.displayUpgrade(upgradeDetail);
        } catch (std::bad_any_cast &e) {
            Logger::error("Sent illegal signal on show upgrade preview");
        }
    });

    subscribe("hide_upgrade_preview", [this](std::any sender, std::any data) {
        infoPanel.clearDisplayUpgrade();
    });

    subscribe("spawn_enemy", [this](std::any sender, std::any data) {
        try {
            EnemySpawnInfo spawnInfo = std::any_cast<EnemySpawnInfo>(data);
            entityManager.addEnemy(factory->createEnemy(spawnInfo.enemyID, 0, spawnInfo.difficultyModifier));
            // Logger::info(std::format("Spawning enemy: {}", enemyID));
        } catch (std::bad_any_cast &e) {
            Logger::error("spawn_enemy: Received illegal signal " +
                          std::string(e.what()));
            return;
        }
    });
    subscribe("resume_game", [this](std::any sender, std::any data) {
        overlay = nullptr;
        Cursor::getInstance().clearCarryingTower();
        Cursor::getInstance().removeRenderImage();
        EnemyPanel::getInstance().clearEnemy();
        upgradeMenu.removeFocus();
    });

    subscribe("quit_level", [this](std::any sender, std::any data) {
        SceneManager::getInstance().enqueueSceneChange("Main menu");
        notify("resume_game");
    });

    subscribe("toggle_sound", [this](std::any sender, std::any data) {
        ResourceManager::getInstance().toggleSound();
    });
    subscribe("toggle_music", [this](std::any sender, std::any data) {
        ResourceManager::getInstance().toggleMusic();
    });

    subscribe("restart_level", [this](std::any sender, std::any data) {
        SceneManager::getInstance().enqueueSceneAdd(
            "Gameplay", LevelFactory::getInstance().getLevel(levelID));
        SceneManager::getInstance().enqueueSceneChange("Gameplay");
    });

    subscribe("show_path",
              [this](std::any sender, std::any data) { renderPath = true; });

    subscribe("hide_path",
              [this](std::any sender, std::any data) { renderPath = false; });
}