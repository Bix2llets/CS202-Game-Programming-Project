#include "Core/Application.hpp"

#include "Base/Constants.hpp"
#include "Core/InputManager.hpp"
#include "Core/MouseState.hpp"
#include "Core/KeyboardState.hpp"
#include "Scene/BlankScene.hpp"
#include "TestMockClasses/SoundClickTrigger.hpp"
#include "Utility/logger.hpp"

#include "Scene/MainMenu.hpp"
#include "Scene/Setting.hpp"

Application::Application()
    : window(sf::VideoMode(
                 {GameConstants::DEFAULT_WINDOW_WIDTH, GameConstants::DEFAULT_WINDOW_HEIGHT}),
             "Rampart remains", sf::Style::Close | sf::Style::Titlebar),
      testTrigger(resourceManager),
      isRunning{true},
      sceneManager{window},
      inputManager{window} {
    if (window.isOpen())
        Logger::success("Window initialization success");
    else
        Logger::error("Window not intitialized");
    window.setFramerateLimit(60);
    resourceManager.loadFont("assets/fonts/League_Spartan/static/LeagueSpartan-Medium.ttf", "LeagueSpartan");
    // * Loading the necessary sounds
    resourceManager.loadSound("assets/sounds/pickupCoin.wav", "coin");
    sceneManager.registerScene<MainMenu>("Main menu", inputManager, resourceManager);
    sceneManager.registerScene<Setting>("Setting", inputManager, resourceManager);
    sceneManager.changeScene("Main menu");
    
}

Application::~Application() {
    if (window.isOpen()) window.close();
    Logger::success("Application exit success");
}

void Application::run() {
    while (isRunning) {
        window.clear(sf::Color::Black);
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                isRunning = false;
            }
            inputManager.handleEvent(event);
            // sceneManager.handleEvent(event);
        }
        // sceneManager.handleInput();
        sceneManager.update();
        sceneManager.render();
        window.display();
    }
}