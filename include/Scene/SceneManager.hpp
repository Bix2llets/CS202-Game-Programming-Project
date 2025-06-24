#pragma once
#include <map>

#include "Scene/Scene.hpp"
#include "Utility/exception.hpp"
#include "Utility/Logger.hpp"
class SceneManager {
   private:
    Scene *currentScene;
    sf::RenderWindow &window;
    std::unordered_map<std::string, std::unique_ptr<Scene>> sceneStorage;

   public:
    SceneManager(sf::RenderWindow &window)
        : currentScene{nullptr}, window{window} {};
    // void changeScene()
    template <typename SceneType>
    void registerScene(const std::string &sceneName) {
        try {
            if (sceneStorage.find(sceneName) == sceneStorage.end()) {
                sceneStorage[sceneName] =
                    std::make_unique<SceneType>(window, sceneName);
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

    void changeScene(const std::string &sceneName);
    const Scene* const &getCurrentScene() { return currentScene; };
    void render();
    void update();
    void handleEvent(std::optional<sf::Event> &event);
    void handleInput();

   private:
    void checkNullptr();
};