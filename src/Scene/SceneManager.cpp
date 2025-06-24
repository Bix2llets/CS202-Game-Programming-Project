#include "Scene/SceneManager.hpp"
#include "Utility/logger.hpp"

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
    catch(GameException exception) {
        Logger::critical("Drawing a non-existent scene");
    }
}

void SceneManager::update() {
    try {
        
        checkNullptr();
        currentScene->update();
    }
    catch(GameException exception) {
        Logger::critical("Updating a non-existent scene");
    }
}

void SceneManager::handleEvent(std::optional<sf::Event> &event) {
    try {
        checkNullptr();
        currentScene->handleEvent(event);
    }
    catch(GameException exception) {
        Logger::critical("Handling event on a non-existent scene");
    }
}

void SceneManager::handleInput() {
    try {
        checkNullptr();
        currentScene->handleInput();
    }
    catch(GameException exception) {
        Logger::critical("Handling input on a non-existent scene");
    }
}

void SceneManager::checkNullptr() {
    if (currentScene == nullptr) throw GameException("Error: nullptr access");
}