#include "Scene/WaveManager.hpp"

#include "Core/JSONLoader.hpp"
#include "Scene/Level.hpp"
#include "Utility/logger.hpp"
WaveManager::WaveManager(Level& parentLevel) : parentLevel{parentLevel} {}

void WaveManager::loadJSON(std::string jsonFileID) {
    nlohmann::json file = JSONLoader::getInstance().getLevel(jsonFileID);
    loadJSON(file);
}

void WaveManager::loadJSON(nlohmann::json jsonFile) {
    auto waveConfiguration = jsonFile["wave"];
    if (waveConfiguration.is_array() == false) {
        Logger::error("Not an array");
        return;
    }

    for (auto waveIt = waveConfiguration.begin();
         waveIt != waveConfiguration.end(); ++waveIt) {
        std::vector<EnemyGroupInfo> enemyGroups;
        for (auto groupIt = waveIt->begin(); groupIt != waveIt->end();
             ++groupIt) {
            EnemyGroupInfo groupInfo;
            groupInfo.id = (*groupIt)["id"];
            groupInfo.quantity = (*groupIt)["quantity"];
            float initialDelay = (*groupIt)["spawn_delay"];
            float spawnInterval = (*groupIt)["internal_delay"];
            groupInfo.initialDelay.setRemainingTime(initialDelay)
                .setTimeInterval(initialDelay)
                .setTimerMode(TimerMode::Single);

            groupInfo.spawnDelay.setRemainingTime(spawnInterval)
                .setTimeInterval(spawnInterval)
                .setTimerMode(TimerMode::Continuous);

            enemyGroups.push_back(groupInfo);
        }
        waveInfo.push_back(enemyGroups);
    }
}

void WaveManager::update() {
    for (auto& groups: waveInfo) {
        for (auto& horde: groups) {
            horde.initialDelay.update();
            horde.spawnDelay.update();

            if (!horde.initialDelay.isAvailable()) continue;
            while(horde.spawnDelay.isAvailable() && horde.quantity) {
                horde.spawnDelay.use();
                parentLevel.notify("spawn_enemy", *this, horde.id);
                horde.quantity--;
            }
            if (horde.quantity == 0) {
                horde.spawnDelay.pause();
                horde.spawnDelay.reset();
            }
        }
            
    }
}