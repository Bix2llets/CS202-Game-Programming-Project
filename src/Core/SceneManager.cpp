#include "Core/SceneManager.hpp"
<<<<<<< HEAD
=======

>>>>>>> Entity
#include "Utility/logger.hpp"

void SceneManager::changeScene(const std::string &sceneName) {
    if (sceneStorage.find(sceneName) == sceneStorage.end()) {
        Logger::error("Switching to non-existent scene");
        return;
    }
<<<<<<< HEAD
    currentScene = sceneStorage[sceneName].get();
=======
    if (currentScene) currentScene->unRegisterComponents();
    currentScene = sceneStorage[sceneName].get();
    if (currentScene) currentScene->registerComponents();
>>>>>>> Entity
}

void SceneManager::render() {
    try {
        checkNullptr();
        window.draw(*currentScene);
<<<<<<< HEAD
    }
    catch(GameException exception) {
=======
    } catch (GameException exception) {
>>>>>>> Entity
        Logger::critical("Drawing a non-existent scene");
    }
}

void SceneManager::update() {
    try {
<<<<<<< HEAD
        
        checkNullptr();
        currentScene->update();
    }
    catch(GameException exception) {
=======
        checkNullptr();
        currentScene->update();
    } catch (GameException exception) {
>>>>>>> Entity
        Logger::critical("Updating a non-existent scene");
    }
}

<<<<<<< HEAD
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
=======
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
>>>>>>> Entity

void SceneManager::checkNullptr() {
    if (currentScene == nullptr) throw GameException("Error: nullptr access");
}