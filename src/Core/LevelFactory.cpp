#include "Core/LevelFactory.hpp"

#include "Utility/logger.hpp"
void LevelFactory::loadConfig(const nlohmann::json& configFile) {
    if (!configFile.is_object())
    {
        Logger::error("Not a JSON object");
        return;
    }
    if (!configFile.contains("id")) {
        Logger::error("Level config missing 'id' key");
        return;
    }
    levelConfiguration[configFile["id"]] = configFile;
    return;
}

nlohmann::json LevelFactory::getConfig() {
    nlohmann::json result = nlohmann::json::array();
    for (auto [name, file]: levelConfiguration)
        result.push_back(file);
    return result;
}

std::unique_ptr<Level> LevelFactory::getLevel(const std::string &levelName) {
    if (levelConfiguration.find(levelName) == levelConfiguration.end()) 
        return nullptr;
    std::unique_ptr<Level> result;
    result = std::make_unique<Level>(window, levelName, sceneManager, inputManager, resourceManager);
    result->loadFromJson(levelConfiguration.at(levelName));
    return std::move(result);
}