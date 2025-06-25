#include "Core/Application.hpp"

#include "Base/Constants.hpp"
#include "Core/InputManager.hpp"
#include "Core/MouseState.hpp"
#include "TestMockClasses/SoundClickTrigger.hpp"
#include "Utility/logger.hpp"
Application::Application()
    : window(sf::VideoMode(
                 {GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT}),
             "Rampart remains"),
      testTrigger(resourceManager),
      isRunning{true},
      sceneManager{window},
      inputManager{window} {
    if (window.isOpen())
        Logger::success("Window initialization success");
    else
        Logger::error("Window not intitialized");
    window.setFramerateLimit(60);
    // * Loading the necessary sounds
    resourceManager.loadSound("assets/sounds/pickupCoin.wav", "coin");
    testTrigger.subscribe(MouseButton::Left, MouseEvent::Click,
                          inputManager.getMouseState());
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
            sceneManager.handleEvent(event);
        }
        sceneManager.handleInput();
        sceneManager.update();
        sceneManager.render();
        window.display();
    }
}