#include "Entity/Tower/Behaviors/Combat/TargetSelector.hpp" 

#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cmath>
#include <limits>

#include "Entity/Tower/Tower.hpp"
#include "Entity/Enemy/Enemy.hpp"

namespace Combat {

std::vector<Enemy*> NearestTargetSelector::selectTarget(
    const Tower* tower, const std::vector<Enemy*>& enemies, int maxTargets) const {
    std::vector<std::pair<Enemy*, float>> validTargets;
    sf::Vector2f towerPos = tower->getPosition();
    float range = tower->getStat(TowerStat::RANGE);

    // Collect all valid targets with their distances
    for (Enemy* enemy : enemies) {
        if (!enemy || !enemy->isAlive()) continue;

        sf::Vector2f enemyPos = enemy->getPosition();
        float distance = std::sqrt(std::pow(towerPos.x - enemyPos.x, 2) + std::pow(towerPos.y - enemyPos.y, 2));

        // Only consider enemies in range
        if (distance <= range) {
            validTargets.emplace_back(enemy, distance);
        }
    }

    // Sort by distance (nearest first)
    std::sort(validTargets.begin(), validTargets.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    // Extract up to maxTargets enemies
    std::vector<Enemy*> selectedTargets;
    selectedTargets.reserve(std::min(maxTargets, static_cast<int>(validTargets.size())));
    
    for (int i = 0; i < std::min(maxTargets, static_cast<int>(validTargets.size())); ++i) {
        selectedTargets.push_back(validTargets[i].first);
    }

    return selectedTargets;
}

std::vector<Enemy*> FarthestTargetSelector::selectTarget(
    const Tower* tower, const std::vector<Enemy*>& enemies, int maxTargets) const {
    std::vector<std::pair<Enemy*, float>> validTargets;
    sf::Vector2f towerPos = tower->getPosition();
    float range = tower->getStat(TowerStat::RANGE);

    // Collect all valid targets with their distances
    for (Enemy* enemy : enemies) {
        if (!enemy || !enemy->isAlive()) continue;

        sf::Vector2f enemyPos = enemy->getPosition();
        float distance = std::sqrt(std::pow(towerPos.x - enemyPos.x, 2) + std::pow(towerPos.y - enemyPos.y, 2));

        // Only consider enemies in range
        if (distance <= range) {
            validTargets.emplace_back(enemy, distance);
        }
    }

    // Sort by distance (farthest first)
    std::sort(validTargets.begin(), validTargets.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    // Extract up to maxTargets enemies
    std::vector<Enemy*> selectedTargets;
    selectedTargets.reserve(std::min(maxTargets, static_cast<int>(validTargets.size())));
    
    for (int i = 0; i < std::min(maxTargets, static_cast<int>(validTargets.size())); ++i) {
        selectedTargets.push_back(validTargets[i].first);
    }

    return selectedTargets;
}

std::vector<Enemy*> LowestHealthTargetSelector::selectTarget(
    const Tower* tower, const std::vector<Enemy*>& enemies, int maxTargets) const {
    std::vector<std::pair<Enemy*, int>> validTargets;
    sf::Vector2f towerPos = tower->getPosition();
    float range = tower->getStat(TowerStat::RANGE);

    // Collect all valid targets with their health
    for (Enemy* enemy : enemies) {
        if (!enemy || !enemy->isAlive()) continue;

        sf::Vector2f enemyPos = enemy->getPosition();
        float distance = std::sqrt(std::pow(towerPos.x - enemyPos.x, 2) + std::pow(towerPos.y - enemyPos.y, 2));

        // Only consider enemies in range
        if (distance <= range) {
            validTargets.emplace_back(enemy, enemy->getHealth());
        }
    }

    // Sort by health (lowest first)
    std::sort(validTargets.begin(), validTargets.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    // Extract up to maxTargets enemies
    std::vector<Enemy*> selectedTargets;
    selectedTargets.reserve(std::min(maxTargets, static_cast<int>(validTargets.size())));
    
    for (int i = 0; i < std::min(maxTargets, static_cast<int>(validTargets.size())); ++i) {
        selectedTargets.push_back(validTargets[i].first);
    }

    return selectedTargets;
}

std::vector<Enemy*> HighestHealthTargetSelector::selectTarget(
    const Tower* tower, const std::vector<Enemy*>& enemies, int maxTargets) const {
    std::vector<std::pair<Enemy*, int>> validTargets;
    sf::Vector2f towerPos = tower->getPosition();
    float range = tower->getStat(TowerStat::RANGE);

    // Collect all valid targets with their health
    for (Enemy* enemy : enemies) {
        if (!enemy || !enemy->isAlive()) continue;

        sf::Vector2f enemyPos = enemy->getPosition();
        float distance = std::sqrt(std::pow(towerPos.x - enemyPos.x, 2) + std::pow(towerPos.y - enemyPos.y, 2));

        // Only consider enemies in range
        if (distance <= range) {
            validTargets.emplace_back(enemy, enemy->getHealth());
        }
    }

    // Sort by health (highest first)
    std::sort(validTargets.begin(), validTargets.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    // Extract up to maxTargets enemies
    std::vector<Enemy*> selectedTargets;
    selectedTargets.reserve(std::min(maxTargets, static_cast<int>(validTargets.size())));
    
    for (int i = 0; i < std::min(maxTargets, static_cast<int>(validTargets.size())); ++i) {
        selectedTargets.push_back(validTargets[i].first);
    }

    return selectedTargets;
}

}  // namespace Combat
