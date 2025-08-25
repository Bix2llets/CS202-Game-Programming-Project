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
#include "Entity/Enemy/EnemySpawnInfo.hpp"
#include "Entity/Factory/StaticEntityFactory.hpp"
#include "Entity/Factory/TowerFactory.hpp"
#include "GUIComponents/EnemyPanel.hpp"
#include "GUIComponents/RectangularButtonBuilder.hpp"
#include "GUIComponents/cursor.hpp"
#include "Gameplay/Difficulty.hpp"
#include "Gameplay/Terrain/TerrainParameter.hpp"
#include "Gameplay/TowerInfoPanel.hpp"
#include "Scene/Overlays/GameoverScreen.hpp"
#include "Scene/Overlays/PauseScreen.hpp"
#include "Scene/Overlays/WinningScreen.hpp"
#include "Utility/CollisionChecker.hpp"
#include "Utility/Scaler.hpp"
#include "Utility/aligner.hpp"
#include "Utility/logger.hpp"
Level::Level() : Level("medium") {}

Level::Level(const std::string &difficultyId)
    : currentWave{0},
      totalWaves{0},
      running{true},
      entityManager{*this},
      menu{budget, this},
      tracker(*this),
      upgradeMenu(*this),
      backgrounds(GameConstants::BLANK_TEXTURE),
      waveManager{*this},
      weatherManager{*this},
      overlay{nullptr},
      randomManager(),
      staticSellMenu(*this),
      waveCounterText(*ResourceManager::getInstance().getFont("text")) {
    // Health and budget will be set by initializeComponentsWithDifficulty()

    // Load specified difficulty
    try {
        difficulty.loadFromConfigFile(difficultyId);
        Logger::success("Loaded difficulty: " + difficultyId + " for level");
    } catch (const std::exception &e) {
        Logger::error("Failed to load difficulty '" + difficultyId +
                      "': " + e.what());
        Logger::warning("Falling back to medium difficulty");
        difficulty.loadFromConfigFile("medium");
    }

    health.setMaxHealth(difficulty.getMaxHealth()).setHealth(difficulty.getMaxHealth());
    // Initialize components with difficulty settings

    MouseState &mouseState = InputManager::getInstance().getMouseState();
    RectangularButtonBuilder builder(*this);
    pauseButton = builder.reset()
                      .setSize(sf::Vector2f{50, 50})
                      .setPosition(sf::Vector2f{
                          GameConstants::MENU_X - 50 - 10,
                          GameConstants::DEFAULT_WINDOW_HEIGHT - 50 - 10})
                      .setCallback([this](RectangularButton *button) {
                          notify("pause_game");
                      })
                      .loadJson("borderless_background_basic")
                      .setBackground(ResourceManager::getInstance().getTexture(
                          "pause_button"))
                      .build();

    nextWaveButton =
        builder.reset()
            .setSize(sf::Vector2f{50, 50})
            .setPosition(
                sf::Vector2f{GameConstants::MENU_X - 50 - 10,
                             GameConstants::DEFAULT_WINDOW_HEIGHT - 100 - 10})
            .setCallback(
                [this](RectangularButton *button) { notify("next_wave"); })
            .loadJson("borderless_background_basic")
            .setBackground(
                ResourceManager::getInstance().getTexture("next_wave_button"))
            .build();
    subscribeCallbacks();
    waveCounterText.setCharacterSize(16);
    waveCounterText.setFillColor(sf::Color::White);
    waveCounterText.setOutlineColor(sf::Color::Black);
    waveCounterText.setOutlineThickness(1.f);

}

Level::~Level() {
    onUnload();
    overlay.reset(nullptr);
}
void Level::update() {
    if (overlay) {
        overlay->update();
        resolveQueue();
        return;
    }
    if (waveManager.isCompleted() && entityManager.getEnemies().empty()) {
        overlay = std::make_unique<WinningScreen>(*this);
        return;
    }
    static sf::Color overlay = sf::Color::Red;
    overlay.a = 127;
    if (!waveManager.isCurrentWaveFinish())
        nextWaveButton->setOverlayColor(overlay);
    else
        nextWaveButton->setOverlayColor(sf::Color::Transparent);
    std::string currentText = waveCounterText.getString();
    std::string nextText = std::format("Wave: {}/{}", waveManager.getCurrentWave() + 1, waveManager.getTotalWaves());
    if (currentText != nextText) {
        waveCounterText.setString(nextText);
        Aligner::align(waveCounterText, HorizontalAlignment::Right, VerticalAlignment::Middle);
        waveCounterText.setPosition(sf::Vector2f{
            GameConstants::MENU_X - 10.f,
             10.f + waveCounterText.getGlobalBounds().size.y / 2.f});
    }
    menu.update();
    infoPanel.update();
    pauseButton->update();
    nextWaveButton->update();

    if (upgradeMenu.isDisplaying()) {
        upgradeMenu.update();
    }

    if (infoPanel.isDisplaying()) {
        infoPanel.update();
    }

    if (staticSellMenu.isDisplaying()) {
        staticSellMenu.update();
    }

    entityManager.update();
    waveManager.update();
    weatherManager.update();

    // Process queued events at the end of the update cycle
    resolveQueue();
}

void Level::draw(sf::RenderTarget &target, sf::RenderStates state) const {
    // drawBackground(target, state);
    Window::getInstance().toggleUserMode();
    // map.render(state);
    Window::getInstance().getRenderWindow().draw(backgrounds, state);

    entityManager.render(state);
    weatherManager.draw(target, state);

    if (upgradeMenu.isDisplaying()) {
        upgradeMenu.render(state);
    }
    if (staticSellMenu.isDisplaying()) {
        staticSellMenu.render(state);
    }

    if (renderPath) path.draw(target, state);

    Window::getInstance().toggleGUIMode();
    menu.render(state);
    target.draw(*pauseButton);
    target.draw(*nextWaveButton);

    if (infoPanel.isDisplaying()) {
        infoPanel.render();
    }
    if (overlay) {
        overlay->render();
    }

    const Weather *currentWaveWeather = weatherManager.getCurrentWeather();
    if (currentWaveWeather) {
        sf::Sprite currentWeatherIcon = currentWaveWeather->getIcon();
        Scaler::scaleSprite(currentWeatherIcon, {32.f, 32.f});
        Aligner::align(currentWeatherIcon, HorizontalAlignment::Center,
                       VerticalAlignment::Middle);
        currentWeatherIcon.setPosition(
            {GameConstants::MENU_X - 16.f - 10.f, 32.f + 16.f + 10.f});
        target.draw(currentWeatherIcon, state);

        sf::RectangleShape backgroundBox;
        backgroundBox.setSize(
            {currentWeatherIcon.getGlobalBounds().size.x + 2.f,
             currentWeatherIcon.getGlobalBounds().size.y + 2.f});
        backgroundBox.setFillColor(sf::Color(0, 0, 0, 0));
        backgroundBox.setOutlineColor(sf::Color::Black);
        backgroundBox.setOutlineThickness(2.f);
        backgroundBox.setPosition(currentWeatherIcon.getPosition());
        Aligner::align(backgroundBox, HorizontalAlignment::Center,
                       VerticalAlignment::Middle);
        target.draw(backgroundBox, state);
        // Logger::debug("Drawing current weather icon");
    }
    const Weather *nextWaveWeather = weatherManager.getNextWeather();
    if (nextWaveWeather) {
        sf::Sprite nextWeatherIcon = nextWaveWeather->getIcon();
        Scaler::scaleSprite(nextWeatherIcon, {32.f, 32.f});
        Aligner::align(nextWeatherIcon, HorizontalAlignment::Center,
                       VerticalAlignment::Middle);
        nextWeatherIcon.setPosition(
            {GameConstants::MENU_X - 16.f - 10.f - 32.f - 10.f, 32.f + 16.f + 10.f});
        target.draw(nextWeatherIcon, state);
        // Logger::debug("Drawing next weather icon");
    }
    target.draw(waveCounterText, state);
}
void Level::loadFromJson(const std::string &pathToFile) {
    nlohmann::json jsonFile = nlohmann::json::parse(std::ifstream(pathToFile));
    loadFromJson(jsonFile);
}

void Level::loadFromJson(const nlohmann::json &jsonFile) {
    backgrounds = sf::Sprite(
        *ResourceManager::getInstance().getTexture(jsonFile["background"]));
    // Note: difficulty is now loaded in constructor, not here
    Logger::success("Level loaded with difficulty: " + difficulty.getName());
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
        {
            if (point.is_array() && point.size() == 3) {
                waypoints.emplace_back(
                    Waypoint{sf::Vector2f{point[0].get<float>() * scale.x,
                                          point[1].get<float>() * scale.y},
                             point[2].get<float>()});
            } else if (point.is_array() && point.size() == 2) {
                waypoints.emplace_back(
                    Waypoint{sf::Vector2f{point[0].get<float>() * scale.x,
                                          point[1].get<float>() * scale.y},
                             1.0});
            } else {
                Logger::error("Invalid waypoint format in JSON");
            }
        }
        // Logger::debug(std::format("Waypoint at {}, {}",
        // point[0].get<float>(),
        //                           point[1].get<float>()));
    }

    path.loadWaypoints(waypoints);
    loadWaves(jsonFile);
    Logger::success("Loaded waypoints");

    weatherManager.setUp();
    // Create EnemyFactory with waypoints - difficulty will be set after obstacle loading
    factory = std::make_unique<EnemyFactory>(waypoints, *this);

    for (nlohmann::json obstacle : jsonFile["obstacles"]) {
        if (obstacle.is_object() == false) {
            continue;
        }
        if (!(obstacle.contains("type") && obstacle["type"].is_string() &&
              obstacle.contains("position") &&
              obstacle["position"].is_object() &&
              obstacle["position"].contains("x") &&
              obstacle["position"]["x"].is_number() &&
              obstacle["position"].contains("y") &&
              obstacle["position"]["y"].is_number())) {
            continue;
        }
        sf::Vector2f position;
        position.x = obstacle["position"]["x"].get<float>() * scale.x;
        position.y = obstacle["position"]["y"].get<float>() * scale.y;
        std::string type = obstacle["type"].get<std::string>();
        entityManager.addStaticEntity(
            StaticEntityFactory::createFromConfigFile(type, *this, position));
        Logger::debug("Added obstacle of type " + type);
    }
    // Apply difficulty settings to EnemyFactory after creation
    factory->setDifficulty(difficulty);
    Logger::debug("Applied " + difficulty.getName() + " difficulty to EnemyFactory");
}

void Level::loadWaves(const nlohmann::json &jsonFile) {
    using namespace nlohmann;
    auto waveConfiguration = jsonFile["wave"];
    if (waveConfiguration.is_array() == false) {
        Logger::error("Not an array");
        return;
    }

    waveManager.loadJSON(jsonFile);
    weatherManager.loadJSON(jsonFile);

    currentWave = 0;
    totalWaves = waveManager.getTotalWaves();
}

void Level::nextWave() {
    if (currentWave < totalWaves) {
        ++currentWave;
        waveManager.nextWave();
        weatherManager.nextWave();
    }
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
    subscribeKeyboard(Key::R, UserEvent::Press,
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

    subscribeMouse(Mouse::Right, UserEvent::Press,
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
    unSubscribeKeyboard(Key::N, UserEvent::Press,
                        InputManager::getInstance().getKeyboardState());
    unSubscribeKeyboard(Key::R, UserEvent::Press,
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
    unSubscribeMouse(Mouse::Right, UserEvent::Press,
                     InputManager::getInstance().getMouseState());
    Logger::debug(std::format("Level onUnLoad done on ", (void *)this));
}

bool Level::isWaveFinished() { return waveManager.isCurrentWaveFinish(); }

bool Level::onKeyEvent(Key key, UserEvent event,
                       const sf::Vector2f &worldPosition,
                       const sf::Vector2f &windowPosition) {
    if (key == Key::Space && event == UserEvent::Press) {
        if (!overlay) {
            overlay = std::make_unique<PauseScreen>(*this);
            Cursor::getInstance().clearCarryingTower();
            Cursor::getInstance().removeRenderImage();
            EnemyPanel::getInstance().clearEnemy();
            upgradeMenu.removeFocus();
        } else {
            overlay = nullptr;
        }
        return true;
    }
    if (key == Key::R && event == UserEvent::Press) {
        if (!overlay) {
            overlay = std::make_unique<GameoverScreen>(*this);
            Cursor::getInstance().clearCarryingTower();
            Cursor::getInstance().removeRenderImage();
            EnemyPanel::getInstance().clearEnemy();
            upgradeMenu.removeFocus();
        }
        return true;
    }

    if (key == Key::G && event == UserEvent::Press) {
        notify("add_currency", 0, Currency(100, 100));
        return true;
    }
    if (key == Key::N && event == UserEvent::Press) {
        nextWave();
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

    if (pauseButton->onMouseEvent(mouse, event, worldPosition,
                                  windowPosition)) {
        return true;
    }

    if (nextWaveButton->onMouseEvent(mouse, event, worldPosition,
                                     windowPosition)) {
        return true;
    }
    if (!infoPanel.isDisplaying()) {
        if (menu.onMouseEvent(mouse, event, worldPosition, windowPosition)) {
            return true;
        }
    } else if (infoPanel.onMouseEvent(mouse, event, worldPosition,
                                      windowPosition))
        return true;

    if (Cursor::getInstance().isDisplaying()) {
        if (isPlacementValid(Cursor::getInstance().getCarryingTowerID(), worldPosition)) {
            Cursor::getInstance().setValidPlacement();
        } else {
            Cursor::getInstance().setInvalidPlacement();
        }
    }

    if (upgradeMenu.onMouseEvent(mouse, event, worldPosition, windowPosition)) {
        return true;
    }
    if (staticSellMenu.onMouseEvent(mouse, event, worldPosition,
                                    windowPosition)) {
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

bool Level::isPlacementValid(std::string towerID, sf::Vector2f worldPosition) {
    Window::getInstance().toggleUserMode();
    if (menu.contains((sf::Vector2f)Window::getInstance()
                          .getRenderWindow()
                          .mapCoordsToPixel(worldPosition)))
        return false;

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

    for (auto &staticEntity : entityManager.getStaticEntities())
        if (staticEntity->intersects(towerBound)) return false;
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
                if (isPlacementValid(Cursor::getInstance().getCarryingTowerID(), 
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
                if (isPlacementValid(Cursor::getInstance().getCarryingTowerID(),
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
        std::pair<std::string, sf::Vector2f> castedData = std::any_cast<std::pair<std::string, sf::Vector2f>>(data);

        if (isPlacementValid(castedData.first, castedData.second)) {
            std::unique_ptr<Tower> newTower =
                std::move(TowerFactory::createFromConfigFile(
                    castedData.first, *this,
                    castedData.second));

            budget.subtractPetroleum(newTower->getCost().getPetroleum().value);
            budget.subtractScraps(newTower->getCost().getScraps().value);

            // Apply current weather effects to the new tower
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
            entityManager.remove(tower);
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

    subscribe("hide_preview", [this](std::any sender, std::any data) {
        infoPanel.clearDisplayUpgrade();
    });

    subscribe("show_evolution", [this](std::any sender, std::any data) {
        try {
            std::string evolutionID = std::any_cast<std::string>(data);
            infoPanel.displayEvolution(evolutionID, this);
        } catch (std::bad_any_cast &e) {
            Logger::error("Sent illegal signal on show evolution");
        }
    });

    subscribe("spawn_enemy", [this](std::any sender, std::any data) {
        try {
            EnemySpawnInfo spawnInfo = std::any_cast<EnemySpawnInfo>(data);
            auto enemy = factory->createEnemy(spawnInfo.enemyID, 0,
                                              spawnInfo.difficultyModifier);

            // Apply current weather effects to the new enemy
            entityManager.addEnemy(std::move(enemy));
            // Logger::info(std::format("Spawning enemy: {}", enemyID));
        } catch (std::bad_any_cast &e) {
            Logger::error("spawn_enemy: Received illegal signal " +
                          std::string(e.what()));
            return;
        }
    });
    subscribe("pause_game", [this](std::any sender, std::any data) {
        overlay = std::make_unique<PauseScreen>(*this);
        Cursor::getInstance().clearCarryingTower();
        Cursor::getInstance().removeRenderImage();
        EnemyPanel::getInstance().clearEnemy();
        upgradeMenu.removeFocus();
        pauseButton->setBackground(
            ResourceManager::getInstance().getTexture("play_button"));
    });
    subscribe("resume_game", [this](std::any sender, std::any data) {
        overlay = nullptr;
        Cursor::getInstance().clearCarryingTower();
        Cursor::getInstance().removeRenderImage();
        EnemyPanel::getInstance().clearEnemy();
        upgradeMenu.removeFocus();
        pauseButton->setBackground(
            ResourceManager::getInstance().getTexture("pause_button"));
    });

    subscribe("quit_level", [this](std::any sender, std::any data) {
        SceneManager::getInstance().enqueueSceneChange("Level Selection");

        if (dynamic_cast<PauseScreen *>(overlay.get())) {
            notify("resume_game");
        }
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

    subscribe("next_wave", [this](std::any sender, std::any data) {
        if (isWaveFinished()) nextWave();
    });
    subscribe("remove_static_entity", [this](std::any sender, std::any data) {
        try {
            StaticEntity *entity = std::any_cast<StaticEntity *>(sender);
            entityManager.remove(entity);
        } catch (std::bad_any_cast &e) {
            Logger::error("Sent illegal signal on remove static entity");
            return;
        }
    });
    subscribe("sell_static_entity", [this](std::any sender, std::any data) {
        try {
            StaticEntity *entity = std::any_cast<StaticEntity *>(sender);
            Currency amount = entity->getRemoveCost();
            amount = amount * 0.6f;
            budget -= amount;
            notify("unfocus_static_entity");
            entityManager.remove(entity);
        } catch (std::bad_any_cast &e) {
            Logger::error("Sent illegal signal on sell static entity");
            return;
        }
    });
    subscribe("focus_static_entity", [this](std::any sender, std::any data) {
        try {
            StaticEntity *entity = std::any_cast<StaticEntity *>(sender);
            staticSellMenu.focusOn(entity);
        } catch (std::bad_any_cast &e) {
            Logger::error("Sent illegal signal on focus static entity");
            return;
        }
    });
    subscribe("unfocus_static_entity", [this](std::any sender, std::any data) {
        staticSellMenu.unFocus();
    });
    subscribe("evolution", [this](std::any sender, std::any data) {
        try {
            RadialUpgradeMenu *menu =
                std::any_cast<RadialUpgradeMenu *>(sender);
            std::string evolutionID = std::any_cast<std::string>(data);
            std::unique_ptr<Tower> evolutionTower =
                TowerFactory::createFromConfigFile(
                    evolutionID, *this,
                    upgradeMenu.getFocusedTower()->getPosition());

            notify("sell_tower", upgradeMenu.getFocusedTower());

            infoPanel.setFocus(evolutionTower.get());
            menu->setFocus(evolutionTower.get());
            entityManager.addTower(std::move(evolutionTower));
            infoPanel.clearDisplayUpgrade();
        } catch (std::bad_any_cast &e) {
            Logger::error("Sent illegal signal on add tower");
            return;
        }
    });
}