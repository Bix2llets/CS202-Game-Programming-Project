#include "Core/Application.hpp"

#include "Base/Constants.hpp"
#include "Core/InputManager.hpp"
#include "Core/JSONLoader.hpp"
#include "Core/KeyboardState.hpp"
#include "Core/MouseState.hpp"
#include "Core/SceneManager.hpp"
#include "Core/TextInputProcessor.hpp"
#include "Core/Window.hpp"
#include "GUIComponents/EnemyPanel.hpp"
#include "GUIComponents/cursor.hpp"
#include "Scene/BlankScene.hpp"
#include "Scene/MainMenu.hpp"
#include "Scene/Mock/TestScene.hpp"
#include "Scene/Mock/TowerRotationMockScene.hpp"
#include "Scene/Setting.hpp"
#include "Utility/logger.hpp"
#include "Core/LevelFactory.hpp"
#include "Scene/LevelSelection.hpp"
Application::Application() : isRunning{true} {
    if (Window::getInstance().getRenderWindow().isOpen())
        Logger::success("Window initialization success");
    else
        Logger::error("Window not intitialized");
    Window::getInstance().getRenderWindow().setFramerateLimit(60);
    Window::getInstance().getRenderWindow().setMouseCursorVisible(false);
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
    Cursor::getInstance().subscribeMouse(
        Mouse::Middle, UserEvent::Move,
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
        LevelFactory::getInstance().loadConfig(levelFile);

    Logger::success("Resource loading");
    SceneManager::getInstance().enqueueSceneAdd("Main menu", std::make_unique<MainMenu>());
     SceneManager::getInstance().enqueueSceneAdd("Setting", std::make_unique<Setting>());
    SceneManager::getInstance().enqueueSceneAdd(
        "Tower Test", std::make_unique<TowerRotationMockScene>());
    SceneManager::getInstance().enqueueSceneAdd(
        "Test Scene", std::make_unique<LevelSelection>());

    // SceneManager::getInstance().enqueueSceneChange(
    //     "Tower Test");  // Start with the tower test scene

    SceneManager::getInstance().enqueueSceneAdd(
        "Gameplay", LevelFactory::getInstance().getLevel("preset_level_serpent_pass"));

    SceneManager::getInstance().enqueueSceneChange("Main menu");
    // SceneManager::getInstance().enqueueSceneChange("Gameplay");
    // sceneManager.enqueueSceneChange("Setting");
}

Application::~Application() {
    if (Window::getInstance().getRenderWindow().isOpen())
        Window::getInstance().getRenderWindow().close();
    Logger::success("Application exit success");
}

void Application::run() {
    sf::Clock clock;

    float fpsTime = 0.f;
    float timeElapsed = 0.f;
    int frameCount = 0;
    sf::Text fpsDisplay(
        *ResourceManager::getInstance().getFont("league_spartan"));
    fpsDisplay.setOrigin({0.f, 0.f});
    fpsDisplay.setPosition({0.f, 0.f});
    fpsDisplay.setFillColor(sf::Color::White);
    fpsDisplay.setOutlineColor(sf::Color::Black);
    while (Window::getInstance().getRenderWindow().isOpen() && isRunning) {
        frameCount++;
        Window::getInstance().toggleUserMode();
        SceneManager::getInstance().updateSceneChange();
        while (auto event =
                   Window::getInstance().getRenderWindow().pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                Window::getInstance().getRenderWindow().close();
                isRunning = false;
            }

            if (event->is<sf::Event::KeyPressed>()) {
                auto keyPress = event->getIf<sf::Event::KeyPressed>();
                if (keyPress) {
                    if (keyPress->code == sf::Keyboard::Key::F1) {
                        SceneManager::getInstance().enqueueSceneChange("Main menu");
                        continue;
                    }
                    if (keyPress->code == sf::Keyboard::Key::F2) {
                        SceneManager::getInstance().enqueueSceneChange("Tower Test");
                        continue;
                    }
                    if (keyPress->code == sf::Keyboard::Key::F3) {
                        SceneManager::getInstance().enqueueSceneChange("Test Scene");
                        continue;
                    }
                    if (keyPress->code == sf::Keyboard::Key::F4) {
                        SceneManager::getInstance().enqueueSceneChange("Custom Creation");
                        continue;
                    }
                }
            }

            InputManager::getInstance().handleEvent(event);
        }
        timeElapsed += clock.getElapsedTime().asSeconds();
        fpsTime += clock.getElapsedTime().asSeconds();
        clock.restart();
        while (timeElapsed > GameConstants::TICK_INTERVAL) {
            timeElapsed -= GameConstants::TICK_INTERVAL;
            SceneManager::getInstance().update();
            EnemyPanel::getInstance().update();
        }
        if (fpsTime > 1.f) {
            fpsTime -= 1.f;
            fpsDisplay.setString(std::to_string(frameCount));
            frameCount = 0;
        }
        Window::getInstance().getRenderWindow().clear(
            sf::Color(183, 183, 183, 255));
        SceneManager::getInstance().render();
        Window::getInstance().toggleGUIMode();
        Window::getInstance().getRenderWindow().draw(fpsDisplay);
        Window::getInstance().getRenderWindow().draw(EnemyPanel::getInstance());
        Window::getInstance().getRenderWindow().draw(Cursor::getInstance());
        Window::getInstance().toggleUserMode();
        Window::getInstance().getRenderWindow().display();
    }
}