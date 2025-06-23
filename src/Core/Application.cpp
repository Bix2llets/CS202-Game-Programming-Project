#include "Core/Application.hpp"

#include "Base/Constants.hpp"
#include "Utility/logger.hpp"
Application::Application()
    : window(sf::VideoMode(
                 {GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT}),
             "Rampart remains"),
      isRunning{true}, sceneManager{window} {
    if (window.isOpen())
        Logger::success("Window initialization success");
    else
        Logger::error("Window not intitialized");
    window.setFramerateLimit(60);
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
            sceneManager.handleEvent(event);
        }
        sceneManager.handleInput();
        sceneManager.update();
        sceneManager.render();
        window.display();
    }
}