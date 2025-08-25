#pragma once

#include <json.hpp>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include "Entity/Modules/Timer.hpp"
#include "GroupInfo.hpp"
class Level;

class WaveManager {
private:
    std::vector<std::vector<EnemyGroupInfo>> waveInfo;
    std::list<std::pair<EnemyGroupInfo, float>> processingHordes;
    Level& parentLevel;
    int currentWave;
    float localDifficulty;

public:
    WaveManager(Level& parentLevel);
    void loadJSON(nlohmann::json jsonFile);
    void loadJSON(std::string jsonFileID);

    void update();

    void setWave(int ID);
    void nextWave();

    inline bool isCurrentWaveFinish() const {
        return processingHordes.empty();
    }
    inline bool isCompleted() const {
        return currentWave == waveInfo.size() - 1 && processingHordes.empty();
    }
    inline int getCurrentWave() const {
        return currentWave;
    }
    inline int getTotalWaves() const {
        return waveInfo.size();
    }

    float getRemainingTime();
};
