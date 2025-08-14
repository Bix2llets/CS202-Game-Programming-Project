/**
 * @file SceneManager.hpp
 * @brief Declares the SceneManager class for managing game scenes.
 */
#pragma once
#include <map>
#include <queue>

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
    struct SceneInfo {
        std::string id;
        std::unique_ptr<Scene> nextScene;
    };
    std::queue<SceneInfo> sceneAddList;
    std::queue<std::string> sceneChangeList;
    Scene *currentScene;  ///< Pointer to the current active scene.
    std::unordered_map<std::string, std::unique_ptr<Scene>>
        sceneStorage;  ///< Storage for all registered scenes.
    SceneManager() : currentScene{nullptr} {};
    SceneManager(const SceneManager &rhs) = delete;
    SceneManager &operator=(const SceneManager &rhs) = delete;

    int currentBuffer = 0;
    std::array<std::unique_ptr<Scene>, 2> levelBuffer;
    public:

    void enqueueSceneAdd(const std::string &id, std::unique_ptr<Scene> scene);
    /**
     * @brief Changes the current scene to the one with the given name.
     * @param sceneName The name of the scene to switch to.
     */
    void enqueueSceneChange(const std::string &sceneName);
    /**
     * @brief Gets a const reference to the current scene pointer.
     * @return Const reference to the current scene pointer.
     */
    const Scene *getCurrentScene() { return currentScene; };
    /**
     * @brief Renders the current scene.
     */
    void render();
    /**
     * @brief Updates the current scene.
     */
    void update();

    static SceneManager &getInstance() {
        static SceneManager instance;
        return instance;
    }

    void setLevel(std::unique_ptr<Scene> scene);

    void switchToLevel();

    void updateSceneChange();

    
    private:
    /**
     * @brief Checks if the current scene pointer is null and throws if so.
     */
    void checkNullptr();
};