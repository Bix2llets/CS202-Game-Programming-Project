#include "Scene/WaveManager.hpp"

#include "Core/JSONLoader.hpp"
#include "Scene/Level.hpp"
#include "Utility/logger.hpp"

#include "Entity/Enemy/EnemySpawnInfo.hpp"
WaveManager::WaveManager(Level& parentLevel) : parentLevel{parentLevel} {
    localDifficulty = 0.f;
    currentWave = -1;
}

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
    for (auto info = processingHordes.begin(); info != processingHordes.end(); info++) {
        EnemyGroupInfo* horde = &info->first;

        horde->initialDelay.update();
        horde->spawnDelay.update();

        if (!horde->initialDelay.isAvailable()) continue;
        while (horde->spawnDelay.isAvailable() && horde->quantity) {
            horde->spawnDelay.use();
            parentLevel.notify("spawn_enemy", *this, EnemySpawnInfo(horde->id, info->second));
            horde->quantity--;
        }
        if (horde->quantity == 0) {
            horde->spawnDelay.pause();
            horde->spawnDelay.reset();
            processingHordes.erase(info);
        }
        
    }
}

void WaveManager::setWave(int ID) {
    if (ID < 0 || ID >= waveInfo.size()) {
        Logger::error("Invalid wave ID: " + std::to_string(ID));
        return;
    };
    currentWave = ID;
    localDifficulty = float(ID) / 10;
    for (auto &group : waveInfo[currentWave]) {
        group.initialDelay.reset();
        group.spawnDelay.reset();
        processingHordes.push_back({std::move(group), localDifficulty});
    }
}

void WaveManager::nextWave() {
    if (currentWave + 1 < waveInfo.size()) {
        setWave(currentWave + 1);
    } else {
        Logger::info("No more waves available, all waves completed.");
        parentLevel.notify("all_waves_completed", *this, currentWave);
    }
}