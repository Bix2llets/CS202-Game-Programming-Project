#include "Scene/SceneManager.h"
#include "Utility/logger.h"
template <typename SceneType>
void SceneManager::registerScene(const std::string &sceneName) {
    try {
        if (sceneStorage.find(sceneName) == sceneStorage.end()) {
            sceneStorage[sceneName] = std::make_unique<SceneType>(window);
        }
    }
    catch (...) {
        Logger::critical("Inserting an object that is not a subclass of Scene");
    }
    return;
}

void SceneManager::changeScene(const std::string &sceneName) {
    if (sceneStorage.find(sceneName) == sceneStorage.end()) {
        Logger::error("Switching to non-existent scene");
        return;
    }
    currentScene = sceneStorage[sceneName].get();
}

void SceneManager::render() {
    try {
        checkNullptr();
        window.draw(*currentScene);
    }
    catch (...) {
        Logger::critical("Drawing a non-existent scene");
    }
}

void SceneManager::update() {
    try {
        
        checkNullptr();
        currentScene->update();
    }
    catch (...) {
        Logger::critical("Updating a non-existent scene");
    }
}

void SceneManager::handleEvent(std::optional<sf::Event> &event) {
    try {
        checkNullptr();
        currentScene->handleEvent(event);
    }
    catch (...) {
        Logger::critical("Handling event on a non-existent scene");
    }
}

void SceneManager::handleInput() {
    try {
        checkNullptr();
        currentScene->handleInput();
    }
    catch (...) {
        Logger::critical("Handling input on a non-existent scene");
    }
}

void SceneManager::checkNullptr() {
    if (currentScene == nullptr) throw 0;
}