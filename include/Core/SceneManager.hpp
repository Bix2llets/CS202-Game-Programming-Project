/**
 * @file SceneManager.hpp
 * @brief Declares the SceneManager class for managing game scenes.
 */
#pragma once
#include <map>

#include "Core/InputManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Scene/Scene.hpp"
#include "Utility/Logger.hpp"
#include "Utility/exception.hpp"
/**
 * @class SceneManager
 * @brief Manages switching, updating, and rendering game scenes.
 */
class SceneManager {
   private:
    Scene *currentScene;       ///< Pointer to the current active scene.
    sf::RenderWindow &window;  ///< Reference to the main window.
    std::unordered_map<std::string, std::unique_ptr<Scene>>
        sceneStorage;  ///< Storage for all registered scenes.
   public:
    /**
     * @brief Constructs a SceneManager for the given window.
     * @param window Reference to the SFML render window.
     */
    SceneManager(sf::RenderWindow &window)
        : currentScene{nullptr}, window{window} {};
    /**
     * @brief Registers a new scene type with a given name.
     * @tparam SceneType The type of the scene to register.
     * @param sceneName The name to register the scene under.
     */
    template <typename SceneType>
    void registerScene(const std::string &sceneName, InputManager &inputManager,
                       ResourceManager &resManager) {
        try {
            if (sceneStorage.find(sceneName) == sceneStorage.end()) {
                sceneStorage[sceneName] =
                    std::make_unique<SceneType>(window, sceneName, *this, inputManager, resManager);
            } else {
                Logger::error(
                    "Name conflict: Inserting a duplicate scene label");
            }
        } catch (...) {
            Logger::critical(
                "Inserting an object that is not a subclass of Scene");
        }
        return;
    }
    /**
     * @brief Changes the current scene to the one with the given name.
     * @param sceneName The name of the scene to switch to.
     */
    void changeScene(const std::string &sceneName);
    /**
     * @brief Gets a const reference to the current scene pointer.
     * @return Const reference to the current scene pointer.
     */
    const Scene *const &getCurrentScene() { return currentScene; };
    /**
     * @brief Renders the current scene.
     */
    void render();
    /**
     * @brief Updates the current scene.
     */
    void update();
    /**
     * @brief Handles an input event for the current scene.
     * @param event Optional SFML event to handle.
     */
    // void handleEvent(std::optional<sf::Event> &event);
    /**
     * @brief Handles real-time input for the current scene.
     */
    // void handleInput();

   private:
    /**
     * @brief Checks if the current scene pointer is null and throws if so.
     */
    void checkNullptr();
};