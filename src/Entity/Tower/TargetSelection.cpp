#include "Entity/Enemy/Enemy.hpp"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <limits>
#include <algorithm>

// Helper function to calculate distance between two positions
float calculateDistance(const sf::Vector2f& pos1, const sf::Vector2f& pos2) {
    return std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2));
}

namespace Combat {

Enemy* NearestTargetSelection::selectTarget(const Tower* tower, const std::vector<Enemy*>& enemies) const {
    Enemy* selectedTarget = nullptr;
    float shortestDistance = std::numeric_limits<float>::max();
    sf::Vector2f towerPos = tower->getPosition();
    float range = tower->getRange();
    
    for (Enemy* enemy : enemies) {
        if (!enemy || !enemy->isAlive()) continue;
        
        sf::Vector2f enemyPos = enemy->getPosition();
        float distance = calculateDistance(towerPos, enemyPos);
        
        // Only consider enemies in range
        if (distance <= range && distance < shortestDistance) {
            shortestDistance = distance;
            selectedTarget = enemy;
        }
    }
    
    return selectedTarget;
}

Enemy* FarthestTargetSelection::selectTarget(const Tower* tower, const std::vector<Enemy*>& enemies) const {
    Enemy* selectedTarget = nullptr;
    float longestDistance = 0.0f;
    sf::Vector2f towerPos = tower->getPosition();
    float range = tower->getRange();
    
    for (Enemy* enemy : enemies) {
        if (!enemy || !enemy->isAlive()) continue;
        
        sf::Vector2f enemyPos = enemy->getPosition();
        float distance = calculateDistance(towerPos, enemyPos);
        
        // Only consider enemies in range
        if (distance <= range && distance > longestDistance) {
            longestDistance = distance;
            selectedTarget = enemy;
        }
    }
    
    return selectedTarget;
}

Enemy* LowestHealthTargetSelection::selectTarget(const Tower* tower, const std::vector<Enemy*>& enemies) const {
    Enemy* selectedTarget = nullptr;
    int lowestHealth = std::numeric_limits<int>::max();
    sf::Vector2f towerPos = tower->getPosition();
    float range = tower->getRange();
    
    for (Enemy* enemy : enemies) {
        if (!enemy || !enemy->isAlive()) continue;
        
        sf::Vector2f enemyPos = enemy->getPosition();
        float distance = calculateDistance(towerPos, enemyPos);
        
        // Only consider enemies in range
        if (distance <= range && enemy->getHealth() < lowestHealth) {
            lowestHealth = enemy->getHealth();
            selectedTarget = enemy;
        }
    }
    
    return selectedTarget;
}

Enemy* HighestHealthTargetSelection::selectTarget(const Tower* tower, const std::vector<Enemy*>& enemies) const {
    Enemy* selectedTarget = nullptr;
    int highestHealth = 0;
    sf::Vector2f towerPos = tower->getPosition();
    float range = tower->getRange();
    
    for (Enemy* enemy : enemies) {
        if (!enemy || !enemy->isAlive()) continue;
        
        sf::Vector2f enemyPos = enemy->getPosition();
        float distance = calculateDistance(towerPos, enemyPos);
        
        // Only consider enemies in range
        if (distance <= range && enemy->getHealth() > highestHealth) {
            highestHealth = enemy->getHealth();
            selectedTarget = enemy;
        }
    }
    
    return selectedTarget;
}

} // namespace Combat
