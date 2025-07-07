#include "Entity/Factory/EnemyFactory.hpp"

#include "Entity/Enemy/Enemy.hpp"
#include "Gameplay/Difficulty.hpp"
#include "Gameplay/Map.hpp"
#include "Entity/Enemy/MovingState.hpp"
EnemyFactory::EnemyFactory(Difficulty difficulty, Map& map,
                           Scene& scene)
    : map{map}, scene{scene} {
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

Enemy EnemyFactory::createBasicEnemy(sf::Vector2f position, sf::Angle angle,
                                     int laneID) {
    Enemy result(scene);
    result.setMaxHealth(100);
    result.setHealth(100);
    result.reward = 7;
    result.setPosition(position);
    result.setRotation(angle);
    result.enemyType = EnemyType::Ground;
    result.setWaypoints(map.getWaypoints(laneID));
    result.speed = 10;
    result.currentState = std::make_unique<MovingState>();

    // * load sprite here
    return result;
}