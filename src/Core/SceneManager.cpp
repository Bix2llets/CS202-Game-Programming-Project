#include "Core/SceneManager.hpp"

#include "Core/Window.hpp"
#include "Utility/logger.hpp"

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


void SceneManager::checkNullptr() {
    if (currentScene == nullptr) throw GameException("Error: nullptr access");
}

void SceneManager::enqueueSceneAdd(const std::string &id,
                                   std::unique_ptr<Scene> scene) {
    sceneAddList.push({id, std::move(scene)});
    Logger::debug("Scene added to queue: " + id);
}

void SceneManager::enqueueSceneChange(const std::string &sceneName) {
    sceneChangeList.push(sceneName);
    Logger::debug("Scene change requested: " + sceneName);
}

void SceneManager::setLevel(std::unique_ptr<Scene> scene) {
    if (dynamic_cast<Level *>(scene.get()) == nullptr) {
        throw std::runtime_error(
            "Cannot add non-Level scene with this method, use enqueueSceneAdd "
            "instead");
    }
    currentBuffer = 1 - currentBuffer;
    levelBuffer[currentBuffer] = std::move(scene);
    Logger::debug(
        "Level set to buffer " + std::to_string(currentBuffer));
}

void SceneManager::switchToLevel() {
    if (currentScene) currentScene->onUnload();
    currentScene = levelBuffer[currentBuffer].get();
    if (currentScene) {
        currentScene->onLoad();
        Logger::info("Switched to level in buffer " + std::to_string(currentBuffer));
    } else {
        Logger::error("Failed to load level from buffer");
    }
}

void SceneManager::updateSceneChange() {
    // Logger::info("Updating scene changes...");
    while (sceneAddList.size()) {
        auto sceneInfo = std::move(sceneAddList.front());
        sceneAddList.pop();
        if (dynamic_cast<Level *>(sceneInfo.nextScene.get())) {
            setLevel(std::move(sceneInfo.nextScene));
            continue;
        }
        if (sceneStorage[sceneInfo.id]) sceneStorage[sceneInfo.id]->onUnload();
        bool shouldRefresh = false;
        if (currentScene == sceneStorage[sceneInfo.id].get()) {
            shouldRefresh = true;
        }
        sceneStorage[sceneInfo.id].reset(sceneInfo.nextScene.release());
        // sceneStorage[sceneInfo.id] = std::move(sceneInfo.nextScene);
        if (shouldRefresh) {
            currentScene = sceneStorage[sceneInfo.id].get();
            if (currentScene)
                currentScene->onLoad();
            else
                Logger::error("Failed to load scene: " + sceneInfo.id);
        }
        Logger::info("Scene added: " + sceneInfo.id);
    }

    while (sceneChangeList.size()) {
        auto sceneName = sceneChangeList.front();
        sceneChangeList.pop();
        if (sceneName == "Gameplay") {
            switchToLevel();
            continue;
        }
        if (sceneStorage.find(sceneName) == sceneStorage.end()) {
            Logger::error("Scene change failed: " + sceneName);
            continue;
        }
        if (currentScene) currentScene->onUnload();
        currentScene = sceneStorage[sceneName].get();
        if (currentScene)
            currentScene->onLoad();
        else
            Logger::error("Failed to load scene: " + sceneName);
        Logger::info("Scene changed to: " + sceneName);
    }
    // Logger::info("Scene changes updated.");
}