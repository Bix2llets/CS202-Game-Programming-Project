#include "Core/Application.hpp"

#include "Base/Constants.hpp"
#include "Core/InputManager.hpp"
#include "Core/JSONLoader.hpp"
#include "Core/KeyboardState.hpp"
#include "Core/MouseState.hpp"
#include "Core/SceneManager.hpp"
#include "Core/Window.hpp"
#include "GUIComponents/cursor.hpp"
#include "Scene/BlankScene.hpp"
#include "Scene/MainMenu.hpp"
#include "Scene/Mock/TowerRotationMockScene.hpp"
#include "Scene/Setting.hpp"
#include "TestMockClasses/SoundClickTrigger.hpp"
#include "Utility/logger.hpp"
Application::Application() : isRunning{true} {
    if (Window::getInstance().isOpen())
        Logger::success("Window initialization success");
    else
        Logger::error("Window not intitialized");
    Window::getInstance().setFramerateLimit(60);
    Window::getInstance().setMouseCursorVisible(false);
    Window::getInstance().setPosition({0, 0});
    JSONLoader::getInstance().loadAll();

    Cursor::getInstance().subscribeMouse(
        Mouse::Left, UserEvent::Move,
        InputManager::getInstance().getMouseState());
    Cursor::getInstance().subscribeMouse(
        Mouse::Right, UserEvent::Move,
        InputManager::getInstance().getMouseState());
    Cursor::getInstance().subscribeMouse(
        Mouse::None, UserEvent::Move,
        InputManager::getInstance().getMouseState());
    // * Loading the necessary sounds
    for (auto [id, soundFile] : JSONLoader::getInstance().getAllSounds())
        ResourceManager::getInstance().loadSound(soundFile);
    for (auto [id, textureFile] : JSONLoader::getInstance().getAllTextures())
        ResourceManager::getInstance().loadTexture(textureFile);
    for (auto [id, musicFile] : JSONLoader::getInstance().getAllMusics())
        ResourceManager::getInstance().loadMusic(musicFile);
    for (auto [id, fontFile] : JSONLoader::getInstance().getAllFonts())
        ResourceManager::getInstance().loadFont(fontFile);
    for (auto [id, levelFile] : JSONLoader::getInstance().getAllLevels())
        levelFactory.loadConfig(levelFile);
    Logger::success("Resource loading");
    SceneManager::getInstance().registerScene<MainMenu>("Main menu");
    SceneManager::getInstance().registerScene<Setting>("Setting");
    SceneManager::getInstance().registerScene<TowerRotationMockScene>(
        "Tower Test");
    SceneManager::getInstance().changeScene(
        "Tower Test");  // Start with the tower test scene
    SceneManager::getInstance().loadLevel(
        "Gameplay", levelFactory.getLevel("exampleLevel"));
    SceneManager::getInstance().changeScene("Main menu");
    // sceneManager.changeScene("Setting");
}

Application::~Application() {
    if (Window::getInstance().isOpen()) Window::getInstance().close();
    Logger::success("Application exit success");
}

void Application::run() {
    sf::Clock clock;
    float timeElapsed = 0.f;
    while (isRunning) {
        while (auto event = Window::getInstance().pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                Window::getInstance().close();
                isRunning = false;
            }

            // Add key to switch between scenes for testing
            if (event->is<sf::Event::KeyPressed>()) {
                auto keyPress = event->getIf<sf::Event::KeyPressed>();
                if (keyPress && keyPress->code == sf::Keyboard::Key::F1) {
                    SceneManager::getInstance().changeScene("Main menu");
                } else if (keyPress &&
                           keyPress->code == sf::Keyboard::Key::F2) {
                    SceneManager::getInstance().changeScene("Tower Test");
                }
            }

            InputManager::getInstance().handleEvent(event);
            // SceneManager::getInstance().handleEvent(event);
        }
        // SceneManager::getInstance().handleInput();
        timeElapsed += clock.restart().asSeconds();
        while (timeElapsed > GameConstants::TICK_INTERVAL) {
            SceneManager::getInstance().update();
            timeElapsed -= GameConstants::TICK_INTERVAL;
        }
        Window::getInstance().clear(sf::Color::Black);
        SceneManager::getInstance().render();
        Window::getInstance().draw(Cursor::getInstance());
        Window::getInstance().display();
    }
}