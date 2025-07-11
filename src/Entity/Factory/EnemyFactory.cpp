#include "Entity/Factory/EnemyFactory.hpp"

#include <SFML/Graphics.hpp>

#include "Core/ResourceManager.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Enemy/MovingState.hpp"
#include "Gameplay/Difficulty.hpp"
#include "Gameplay/Map.hpp"
#include "Scene/Scene.hpp"
EnemyFactory::EnemyFactory(Difficulty difficulty, Map &map, Scene &scene,
                           JSONLoader &loader, ResourceManager &resManager)
    : map(map), scene(scene), loader{loader}, resManager{resManager} {
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

        default:
            break;
    }
}

Enemy EnemyFactory::createEnemy(const std::string &id, float distance,
                                int laneID) {
    nlohmann::json enemyFile = (loader.getEnemy(id));
    if (!enemyFile.contains("sprite") || !enemyFile.contains("stats") ||
        !enemyFile.contains("type"))
        throw std::runtime_error("Missing required enemy fields in JSON");
    Enemy result(scene);
    result.animation.loadJson(resManager, enemyFile["sprite"]);
    result.path.setWaypoints(map.getWaypoints(laneID));
    result.path.setDistanceFromStart(distance);
    result.path.setSpeed(enemyFile["stats"]["speed"]);
    result.health.setMaxHealth(enemyFile["stats"]["maxHealth"]);
    result.health.setHealth(result.health.getMaxHealth());
    result.healTimer.setTimerMode(TimerMode::Continuous);
    result.healAmount = enemyFile["stats"]["healAmount"];
    result.enemyType =
        (enemyFile["type"] == "land" ? EnemyType::Ground : EnemyType::Aerial);

    result.reward = static_cast<float>(enemyFile["stats"]["reward"]) * rewardMultiplier;
    return result;
}
