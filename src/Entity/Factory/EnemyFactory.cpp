#include "Entity/Factory/EnemyFactory.hpp"

#include <SFML/Graphics.hpp>

#include "Core/JSONLoader.hpp"
#include "Core/ResourceManager.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Enemy/MovingState.hpp"
#include "Gameplay/Difficulty.hpp"
#include "Gameplay/Path.hpp"
#include "Scene/Scene.hpp"
EnemyFactory::EnemyFactory(Terrain &map, Scene &scene)
    : map(map), scene(scene) {
        rewardMultiplier = 1.f;
        speedMultiplier = 1.f;
        healthMultiplier = 1.f;
    }
void EnemyFactory::setDifficulty(Difficulty difficulty) {
    switch (difficulty) {
        case Difficulty::Easy: {
            rewardMultiplier = 1.2f;
            speedMultiplier = 0.9f;
            healthMultiplier = 0.95f;
            return;
        }
        case Difficulty::Medium: {
            rewardMultiplier = 1.f;
            speedMultiplier = 1.f;
            healthMultiplier = 1.f;
            return;
        }
        case Difficulty::Hard: {
            rewardMultiplier = 0.95f;
            speedMultiplier = 1.05f;
            healthMultiplier = 1.05f;
            return;
        }
        
        default: {
            rewardMultiplier = 1.f;
            speedMultiplier = 1.f;
            healthMultiplier = 1.f;
            return;
            
        }
    }
}

std::unique_ptr<Enemy> EnemyFactory::createEnemy(const std::string &id,
                                                 float distance) {
    nlohmann::json enemyFile = (JSONLoader::getInstance().getEnemy(id));
    if (!enemyFile.contains("sprite") || !enemyFile.contains("stats") ||
        !enemyFile.contains("type"))
        throw std::runtime_error("Missing required enemy fields in JSON");
    std::unique_ptr<Enemy> result(new Enemy(scene));
    result->animation.loadJson(enemyFile["sprite"]);
    result->path.setWaypoints(map.getPath());
    result->path.setDistanceFromStart(distance);
    result->path.setSpeed(enemyFile["stats"]["speed"]);
    result->health.setMaxHealth(enemyFile["stats"]["max_health"]);
    result->health.setHealth(result->health.getMaxHealth());
    result->healTimer.setTimeInterval(enemyFile["stats"]["heal_interval"])
        .setTimerMode(TimerMode::Single)
        .setRemainingTime(enemyFile["stats"]["heal_interval"]);
    result->healAmount = enemyFile["stats"]["heal_amount"];
    result->enemyType =
        (enemyFile["type"] == "land" ? EnemyType::Ground : EnemyType::Aerial);

    result->petroleumReward =
        static_cast<float>(enemyFile["stats"]["reward"]["petroleum"]) * rewardMultiplier;
    result->scrapReward =
        static_cast<float>(enemyFile["stats"]["reward"]["scrap"]) * rewardMultiplier;

    result->name = enemyFile["name"];
    return std::move(result);
}
