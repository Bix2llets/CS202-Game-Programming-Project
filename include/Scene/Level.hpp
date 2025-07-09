#pragma once
#include "Gameplay/Map.hpp"
#include "Gameplay/Waypoint.hpp"
#include "Scene/Scene.hpp"
#include <json.hpp>
#include "Gameplay/Map.hpp"
#include "Scene/GroupInfo.hpp"
class SceneManager;
class InputManager;
class ResourceManager;
class Level : public Scene {
    private:

    std::string levelID;
    void loadWaypoints(const nlohmann::json& jsonFile);
    void loadWaves(const nlohmann::json& jsonFile);
    void loadLevelID(const nlohmann::json& jsonfile);
    Map map;
    std::vector<std::vector<EnemyGroupInfo>> waveInfo;
    public:

    Level(sf::RenderWindow &window, const std::string &name,
              SceneManager &parentManager, InputManager &inputManager,
              ResourceManager &resourceManager);

    void update();
    void draw(sf::RenderTarget &target, sf::RenderStates state) const;
    void loadFromJson(const std::string &pathToFile);
    void loadFromJson(const nlohmann::json& jsonFile);
    inline const std::string getID() const { return "Level" + levelID; }

    void registerComponents() override;
    void unRegisterComponents() override;

};