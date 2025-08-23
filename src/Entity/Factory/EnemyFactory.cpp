#include "Entity/Factory/EnemyFactory.hpp"

#include <SFML/Graphics.hpp>

#include "Core/JSONLoader.hpp"
#include "Core/ResourceManager.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Enemy/MovingState.hpp"
#include "Entity/Modules/Effects/EntityEffect.hpp"
#include "Gameplay/Difficulty.hpp"
#include "Gameplay/Terrain/Path.hpp"
#include "Scene/Scene.hpp"
EnemyFactory::EnemyFactory(std::vector<Waypoint> waypoints, Scene &scene)
    : waypoints{waypoints}, scene(scene) {
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
                                                 float distance,
                                                 float localDifficulty) {
    nlohmann::json enemyFile = (JSONLoader::getInstance().getEnemy(id));
    if (!enemyFile.contains("sprite") || !enemyFile.contains("stats") ||
        !enemyFile.contains("type"))
        throw std::runtime_error("Missing required enemy fields in JSON");
    std::unique_ptr<Enemy> result(new Enemy(scene));
    result->attackDisplayTimer.setTimeInterval(0.5)
        .setRemainingTime(0)
        .setTimerMode(TimerMode::Single);
    result->animation.loadJson(enemyFile["sprite"]);
    result->path.setWaypoints(&waypoints);
    result->path.setDistanceFromStart(distance);
    result->path.setSpeed(enemyFile["stats"]["speed"].get<float>() *
                          (1 + localDifficulty / 10) * speedMultiplier);
    result->health.setMaxHealth(enemyFile["stats"]["max_health"].get<float>() *
                                (1 + localDifficulty) * healthMultiplier);
    result->health.setHealth(result->health.getMaxHealth());
    result->enemyType =
        (enemyFile["type"] == "land" ? EnemyType::Ground : EnemyType::Aerial);

    result->petroleumReward =
        static_cast<float>(enemyFile["stats"]["reward"]["petroleum"]) *
        rewardMultiplier;
    result->scrapReward =
        static_cast<float>(enemyFile["stats"]["reward"]["scrap"]) *
        rewardMultiplier;

    result->name = enemyFile["name"];

    nlohmann::json stats = enemyFile["stats"];
    result->originalStats.setStat(EnemyStat::FIRE_RESISTANCE,
                                  stats[EnemyStat::FIRE_RESISTANCE]);
    result->originalStats.setStat(EnemyStat::MAX_HEALTH,
                                  stats[EnemyStat::MAX_HEALTH]);
    result->originalStats.setStat(EnemyStat::REGENERATION,
                                  stats[EnemyStat::REGENERATION]);
    result->originalStats.setStat(EnemyStat::RESISTANCE,
                                  stats[EnemyStat::RESISTANCE]);
    result->originalStats.setStat(EnemyStat::SPEED, stats[EnemyStat::SPEED]);

    unsigned long long effectID = (unsigned long long)(1e18);
    int infiniteTime = INT_MAX;
    result->applyEffect(EffectType::FireResistance,
                        stats[EnemyStat::FIRE_RESISTANCE], infiniteTime,
                        effectID);
    result->applyEffect(EffectType::Resistance, stats[EnemyStat::RESISTANCE],
                        infiniteTime, effectID);
    float healAmount = enemyFile["stats"]["heal_amount"].get<float>() *
                       (1 + localDifficulty / 10);
    result->applyEffect(EffectType::Regeneration, healAmount, infiniteTime,
                        effectID);

    std::unordered_map<std::string, float> spawnEffect =
        enemyFile["spawn_effect"].get<std::unordered_map<std::string, float>>();

    EntityStat spawnEffects;
    for (auto [id, value] : spawnEffect) {
        spawnEffects.setStat(id, value);
    }
    result->applyEffects(infiniteTime, spawnEffects);
    return std::move(result);
}
