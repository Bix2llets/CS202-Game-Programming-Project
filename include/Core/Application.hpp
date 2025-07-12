/**
 * @file Application.hpp
 * @brief Declares the Application class, the main entry point for the game.
 */
#pragma once
#include <SFML/Graphics.hpp>

#include "Core/InputManager.hpp"
#include "Core/JSONLoader.hpp"
#include "Core/LevelFactory.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/SceneManager.hpp"
#include "TestMockClasses/SoundClickTrigger.hpp"
/**
 * @class Application
 * @brief Main application class that manages the game loop and core systems.
 */
class Application {
   private:
    sf::RenderWindow window;    ///< The main game window.
    SceneManager sceneManager;  ///< Manages game scenes.
    ResourceManager
        resourceManager;        ///< Manages resources (textures, sounds, etc.).
    InputManager inputManager;  ///< Handles input events.
    SoundClickTrigger testTrigger;  ///< Test trigger for sound on click.
    JSONLoader loader;
    LevelFactory levelFactory;
    bool isRunning;  ///< Indicates if the application is running.
   public:
    /**
     * @brief Constructs the Application and initializes core systems.
     */
    Application();
    /**
     * @brief Runs the main game loop.
     */
    void run();
    /**
     * @brief Destructor.
     */
    ~Application();
};