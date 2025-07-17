/**
 * @file SceneManager.hpp
 * @brief Declares the SceneManager class for managing game scenes.
 */
#pragma once
#include <map>

#include "Core/ResourceManager.hpp"
#include "Scene/Level.hpp"
#include "Scene/Scene.hpp"
#include "Utility/Logger.hpp"
#include "Utility/exception.hpp"
/**
 * @class SceneManager
 * @brief Manages switching, updating, and rendering game scenes.
 */
class SceneManager {
   private:
    Scene *currentScene;  ///< Pointer to the current active scene.
    std::unordered_map<std::string, std::unique_ptr<Scene>>
        sceneStorage;  ///< Storage for all registered scenes.
   public:
    /**
     * @brief Constructs a SceneManager for the given window.
     * @param window Reference to the SFML render window.
     */
    SceneManager() : currentScene{nullptr} {};
    /**
     * @brief Registers a new scene type with a given name.
     * @tparam SceneType The type of the scene to register.
     * @param sceneName The name to register the scene under.
     */
    template <typename SceneType>
    void registerScene(const std::string &sceneName,
                       ResourceManager &resManager) {
        try {
            if (sceneStorage.find(sceneName) == sceneStorage.end()) {
                sceneStorage[sceneName] =
                    std::make_unique<SceneType>(*this, resManager);
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
     * @brief Load a level from a unique poiner of level
     * @param ID The scene's ID
     * @param level the unique_ptr of the Level
     */
    void loadLevel(std::string ID, std::unique_ptr<Level> level);

   private:
    /**
     * @brief Checks if the current scene pointer is null and throws if so.
     */
    void checkNullptr();
};