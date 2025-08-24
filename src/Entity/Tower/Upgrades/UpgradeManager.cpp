/**
 * @file UpgradeManager.cpp
 * @brief Implementation of the UpgradeManager class.
 */

#include "Entity/Tower/Upgrades/UpgradeManager.hpp"
#include "Entity/Tower/Tower.hpp"
#include <algorithm>
#include "Core/ResourceManager.hpp"
#include "Core/JSONLoader.hpp"

UpgradeResult UpgradeManager::upgrade(int typeId) {
    // Check if upgrade type exists
    auto upgradeTypeIt = upgradeTypes.find(typeId);
    if (upgradeTypeIt == upgradeTypes.end()) {
        return UpgradeResult::InvalidUpgradeType;
    }

    const UpgradeType* upgradeType = upgradeTypeIt->second.get();
    int currentLevel = getCurrentLevel(typeId);
    int nextLevel = currentLevel + 1;

    // Check if max level reached for this upgrade type
    if (nextLevel > upgradeType->getMaxLevel()) {
        return UpgradeResult::MaxLevelReached;
    }

    // Check if total upgrade limit reached
    if (isTotalUpgradeLimitReached()) {
        return UpgradeResult::MaxTotalUpgrades;
    }

    // Get upgrade details for next level
    const UpgradeDetails* details = upgradeType->getLevelDetails(nextLevel);
    if (!details) {
        return UpgradeResult::InvalidLevel;
    }


    Currency upgradeCost = upgradeType->getLevelDetails(nextLevel)->cost;
    ownerTower->addTotalCost(upgradeCost);
    applyUpgrade(typeId, nextLevel);
    return UpgradeResult::Success;
}

float UpgradeManager::getTotalStatBonus(const std::string& statName) const {
    float totalBonus = 0.0f;

    for (const auto& [typeId, currentLevel] : currentLevels) {
        auto upgradeTypeIt = upgradeTypes.find(typeId);
        if (upgradeTypeIt != upgradeTypes.end()) {
            const UpgradeType* upgradeType = upgradeTypeIt->second.get();
            totalBonus += upgradeType->getTotalBonusForStat(statName, currentLevel);
        }
    }

    return totalBonus;
}

EntityStat UpgradeManager::getAllUpgradeBonuses() const {
    EntityStat totalBonuses;

    for (const auto& [typeId, currentLevel] : currentLevels) {
        auto upgradeTypeIt = upgradeTypes.find(typeId);
        if (upgradeTypeIt != upgradeTypes.end()) {
            const UpgradeType* upgradeType = upgradeTypeIt->second.get();
            
            // Add bonuses from each level of this upgrade type
            for (int level = 1; level <= currentLevel; ++level) {
                const UpgradeDetails* details = upgradeType->getLevelDetails(level);
                if (details) {
                    totalBonuses += details->getBonusStats();
                }
            }
        }
    }

    return totalBonuses;
}

bool UpgradeManager::canUpgrade(int typeId, const Currency& playerCurrency) const {
    // Check if upgrade type exists
    auto upgradeTypeIt = upgradeTypes.find(typeId);
    if (upgradeTypeIt == upgradeTypes.end()) {
        return false;
    }

    const UpgradeType* upgradeType = upgradeTypeIt->second.get();
    int currentLevel = getCurrentLevel(typeId);
    int nextLevel = currentLevel + 1;

    // Check if max level reached
    if (nextLevel > upgradeType->getMaxLevel()) {
        return false;
    }

    // Check if total upgrade limit reached
    if (isTotalUpgradeLimitReached()) {
        return false;
    }

    // Get upgrade details for next level
    const UpgradeDetails* details = upgradeType->getLevelDetails(nextLevel);
    if (!details) {
        return false;
    }

    // Check if player has sufficient funds
    return playerCurrency.canAfford(details->cost);
}

const UpgradeDetails* UpgradeManager::getNextUpgradeDetail(int typeId) const {
    auto upgradeTypeIt = upgradeTypes.find(typeId);
    if (upgradeTypeIt == upgradeTypes.end()) {
        return nullptr;
    }

    const UpgradeType* upgradeType = upgradeTypeIt->second.get();
    int currentLevel = getCurrentLevel(typeId);
    int nextLevel = currentLevel + 1;

    if (nextLevel > upgradeType->getMaxLevel()) {
        return nullptr;
    }

    return upgradeType->getLevelDetails(nextLevel);
}

std::vector<std::string> UpgradeManager::getAvailableEvolutions() const {
    std::vector<std::string> evolutions;

    for (const auto& [typeId, currentLevel] : currentLevels) {
        auto upgradeTypeIt = upgradeTypes.find(typeId);
        if (upgradeTypeIt != upgradeTypes.end()) {
            const UpgradeType* upgradeType = upgradeTypeIt->second.get();
            
            // Check if this upgrade type is at max level and has evolution
            if (currentLevel == upgradeType->getMaxLevel() && !upgradeType->getEvolveTo().empty()) {
                evolutions.push_back(upgradeType->getEvolveTo());
            }
        }
    }

    // Remove duplicates (in case multiple upgrade paths lead to same evolution)
    std::sort(evolutions.begin(), evolutions.end());
    evolutions.erase(std::unique(evolutions.begin(), evolutions.end()), evolutions.end());

    return evolutions;
}

void UpgradeManager::resetAllUpgrades() {
    for (auto& [typeId, level] : currentLevels) {
        level = 0;
    }
    totalUpgradeCount = 0;
}

void UpgradeManager::applyUpgrade(int typeId, int newLevel) {
    int oldLevel = getCurrentLevel(typeId);
    currentLevels[typeId] = newLevel;
    
    // Update total upgrade count
    totalUpgradeCount += (newLevel - oldLevel);

    // Note: Stat recalculation will happen when Tower calls getTotalStatBonus()
    // This keeps the upgrade system decoupled from the tower's stat system
}

bool UpgradeManager::canEvolve(int typeId) const {
    auto upgradeTypeIt = upgradeTypes.find(typeId);
    if (upgradeTypeIt == upgradeTypes.end()) {
        return false;
    }

    const UpgradeType* upgradeType = upgradeTypeIt->second.get();
    int currentLevel = getCurrentLevel(typeId);

    try {

        JSONLoader::getInstance().getTower(upgradeType->getEvolveTo());
    }
    catch (...) {
        return false;
    }
    // Check if this upgrade type is at max level and has an evolution path
    return currentLevel == upgradeType->getMaxLevel() && !upgradeType->getEvolveTo().empty();
}

std::string UpgradeManager::getEvolveTo(int typeId) const {
    auto upgradeTypeIt = upgradeTypes.find(typeId);
    if (upgradeTypeIt == upgradeTypes.end()) {
        return "";
    }

    const UpgradeType* upgradeType = upgradeTypeIt->second.get();
    return upgradeType->getEvolveTo();
}