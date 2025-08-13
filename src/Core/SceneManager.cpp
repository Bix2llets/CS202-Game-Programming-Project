#include "Core/SceneManager.hpp"
#include "Utility/logger.hpp"
#include "Core/Window.hpp"

void SceneManager::changeScene(const std::string &sceneName) {
    if (sceneStorage.find(sceneName) == sceneStorage.end()) {
        Logger::error("Switching to non-existent scene");
        return;
    }
    if (currentScene) currentScene->onUnload();
     currentScene = sceneStorage[sceneName].get();
    if (currentScene) currentScene->onLoad();
}

void SceneManager::render() {
    try {
        checkNullptr();
        Window::getInstance().getRenderWindow().draw(*currentScene);
    } catch (GameException exception) {
        Logger::critical("Drawing a non-existent scene");
    }
}

void SceneManager::update() {
    try {
        checkNullptr();
        currentScene->update();
    } catch (GameException exception) {
        Logger::critical("Updating a non-existent scene");
    }
}

// void SceneManager::handleEvent(std::optional<sf::Event> &event) {
//     try {
//         checkNullptr();
//         currentScene->handleEvent(event);
//     }
//     catch(GameException exception) {
//         Logger::critical("Handling event on a non-existent scene");
//     }
// }

// void SceneManager::handleInput() {
//     try {
//         checkNullptr();
//         currentScene->handleInput();
//     }
//     catch(GameException exception) {
//         Logger::critical("Handling input on a non-existent scene");
//     }
// }

void SceneManager::checkNullptr() {
    if (currentScene == nullptr) throw GameException("Error: nullptr access");
}

void SceneManager::loadLevel(std::string ID, std::unique_ptr<Level> level) {

    sceneStorage[ID] = std::move(level);
}

void SceneManager::createLevel(std::string ID) {
    if (currentScene == sceneStorage[ID].get()) {
        changeScene("Main Menu");
        Logger::info("Switching to Main Menu before creating a new level");
    }
    auto level = std::make_unique<Level>();
    loadLevel(ID, std::move(level));   
}