#pragma once

#include <json.hpp>
#include <string>
#include <vector>

#include "Entity/Modules/Timer.hpp"
#include "GroupInfo.hpp"
class Level;

class WaveManager {
    private:
    std::vector<std::vector<EnemyGroupInfo>> waveInfo;
    Level& parentLevel;

    public:
    WaveManager(Level& parentLevel);
    void loadJSON(nlohmann::json jsonFile);
    void loadJSON(std::string jsonFileID);

    void update();
};
