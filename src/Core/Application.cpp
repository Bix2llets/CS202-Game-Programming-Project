#include "Core/Application.hpp"

#include <fstream>

#include "Base/Constants.hpp"
#include "Core/InputManager.hpp"
#include "Core/KeyboardState.hpp"
#include "Core/LevelFactory.hpp"
#include "Core/MouseState.hpp"
#include "Scene/BlankScene.hpp"
#include "Scene/Level.hpp"
#include "Scene/MainMenu.hpp"
#include "Scene/Setting.hpp"
#include "TestMockClasses/SoundClickTrigger.hpp"
#include "Utility/logger.hpp"
Application::Application()
    : window(sf::VideoMode({GameConstants::DEFAULT_WINDOW_WIDTH,
                            GameConstants::DEFAULT_WINDOW_HEIGHT}),
             "Rampart remains", sf::Style::Close | sf::Style::Titlebar),
      testTrigger(resourceManager),
      isRunning{true},
      sceneManager{window},
      inputManager{window} {
    LevelFactory levelFactory(window, sceneManager, inputManager,
                              resourceManager);
    if (window.isOpen())
        Logger::success("Window initialization success");
    else
        Logger::error("Window not intitialized");
    window.setFramerateLimit(60);
    resourceManager.loadFont(
        "assets/fonts/League_Spartan/static/LeagueSpartan-Medium.ttf",
        "LeagueSpartan");
        std::fstream exampleJson("content/level/example.json");
    nlohmann::json testLevelConfig =
        nlohmann::json::parse(exampleJson);
    levelFactory.loadConfig(testLevelConfig);
    resourceManager.loadTexture("assets/texture/enemy/basic.png", "BasicEnemy");
    // * Loading the necessary sounds
    resourceManager.loadSound("assets/sounds/pickupCoin.wav", "coin");
    sceneManager.registerScene<MainMenu>("Main menu", inputManager,
                                         resourceManager);
    sceneManager.registerScene<Setting>("Setting", inputManager,
                                        resourceManager);
    sceneManager.changeScene("Main menu");
    sceneManager.registerScene<Level>("Gameplay", inputManager,
                                      resourceManager);
    sceneManager.addLevel(levelFactory.getLevel("exampleLevel"));
}

Application::~Application() {
    if (window.isOpen()) window.close();
    Logger::success("Application exit success");
}

void Application::run() {
    sf::Clock clock;
    float timeElapsed = 0.f;
    while (isRunning) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                isRunning = false;
            }
            inputManager.handleEvent(event);
            // sceneManager.handleEvent(event);
        }
        // sceneManager.handleInput();
        timeElapsed += clock.restart().asSeconds();
        while (timeElapsed > GameConstants::TICK_INTERVAL) {
            sceneManager.update();
            timeElapsed -= GameConstants::TICK_INTERVAL;
        }
        window.clear(sf::Color::Black);
        sceneManager.render();
        window.display();
    }
}