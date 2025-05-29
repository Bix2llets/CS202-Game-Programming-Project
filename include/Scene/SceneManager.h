#pragma once
#include "Scene/Scene.h"
#include <map>
class SceneManager{
    private:
    Scene* currentScene;
    sf::RenderWindow &window;
    std::unordered_map<std::string, std::unique_ptr<Scene>> sceneStorage;
    public:
    SceneManager(sf::RenderWindow &window) : currentScene{nullptr}, window{window} {};
    // void changeScene()
    template<typename SceneType>
    void registerScene(const std::string& sceneName);
    void changeScene(const std::string &sceneName);

    void render();
    void update();
    void handleEvent(std::optional<sf::Event> &event);
    void handleInput();

    private:
    void checkNullptr();

};