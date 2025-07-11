#include <SFML/Graphics.hpp>

#include "Entity/Factory/EnemyFactory.hpp"

#include "Entity/Enemy/Enemy.hpp"
#include "Gameplay/Difficulty.hpp"
#include "Gameplay/Map.hpp"
#include "Entity/Enemy/MovingState.hpp"
#include "Core/ResourceManager.hpp"
EnemyFactory::EnemyFactory(Difficulty difficulty, Map& map, Scene& scene, ResourceManager &resourceManager)
    : map(map), scene(scene), resourceManager(resourceManager) {
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

