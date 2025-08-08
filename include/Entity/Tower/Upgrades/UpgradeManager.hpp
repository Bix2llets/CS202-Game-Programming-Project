/**
 * @file UpgradeManager.hpp
 * @brief Defines the UpgradeManager class for managing tower upgrades.
 */

#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include "UpgradeType.hpp"
#include "Gameplay/Currency.hpp"

// Forward declaration
class Tower;

/**
 * @enum UpgradeResult
 * @brief Result of an upgrade attempt.
 */
enum class UpgradeResult {
    Success,                ///< Upgrade was successful
    InsufficientFunds,      ///< Not enough currency for upgrade
    MaxLevelReached,        ///< Upgrade type is already at max level
    MaxTotalUpgrades,       ///< Total upgrade limit reached
    InvalidUpgradeType,     ///< Upgrade type doesn't exist
    InvalidLevel           ///< Level is invalid for this upgrade type
};

/**
 * @class UpgradeManager
 * @brief Manages all upgrades for a specific tower instance.
 * 
 * Tracks current upgrade levels, enforces upgrade limits, and applies
 * stat bonuses from upgrades.
 */
class UpgradeManager {
private:
    Tower* ownerTower;                                          ///< Pointer to the tower that owns this manager
    std::unordered_map<int, std::unique_ptr<UpgradeType>> upgradeTypes; ///< Map of upgrade type ID -> UpgradeType
    std::unordered_map<int, int> currentLevels;                 ///< Map of upgrade type ID -> current level
    int maxTotalUpgrades;                                       ///< Maximum total upgrades allowed
    int totalUpgradeCount;                                      ///< Current total number of upgrades

public:
    /**
     * @brief Constructor.
     * @param tower Pointer to the tower that owns this manager.
     */
    explicit UpgradeManager(Tower* tower) 
        : ownerTower(tower), maxTotalUpgrades(0), totalUpgradeCount(0) {}

    /**
     * @brief Destructor.
     */
    ~UpgradeManager() = default;

    /**
     * @brief Set the maximum total upgrades allowed.
     * @param maxUpgrades Maximum number of upgrades across all types.
     */
    void setMaxTotalUpgrades(int maxUpgrades) { maxTotalUpgrades = maxUpgrades; }

    /**
     * @brief Add an upgrade type to this manager.
     * @param typeId Unique identifier for the upgrade type (1, 2, 3, etc.).
     * @param upgradeType Unique pointer to the upgrade type.
     */
    void addUpgradeType(int typeId, std::unique_ptr<UpgradeType> upgradeType) {
        upgradeTypes[typeId] = std::move(upgradeType);
        currentLevels[typeId] = 0; // Initialize to level 0 (no upgrades)
    }

    /**
     * @brief Attempt to upgrade a specific upgrade type.
     * @param typeId The upgrade type ID to upgrade.
     * @return Result of the upgrade attempt.
     */
    UpgradeResult upgrade(int typeId);

    /**
     * @brief Get the current level of a specific upgrade type.
     * @param typeId The upgrade type ID.
     * @return Current level (0 if not found or no upgrades).
     */
    int getCurrentLevel(int typeId) const {
        auto it = currentLevels.find(typeId);
        return it != currentLevels.end() ? it->second : 0;
    }

    /**
     * @brief Get the upgrade type by ID.
     * @param typeId The upgrade type ID.
     * @return Pointer to upgrade type, or nullptr if not found.
     */
    const UpgradeType* getUpgradeType(int typeId) const {
        auto it = upgradeTypes.find(typeId);
        return it != upgradeTypes.end() ? it->second.get() : nullptr;
    }

    /**
     * @brief Get all available upgrade types.
     * @return Reference to the upgrade types map.
     */
    const std::unordered_map<int, std::unique_ptr<UpgradeType>>& getAllUpgradeTypes() const {
        return upgradeTypes;
    }

    /**
     * @brief Get all current upgrade levels.
     * @return Reference to the current levels map.
     */
    const std::unordered_map<int, int>& getCurrentLevels() const {
        return currentLevels;
    }

    /**
     * @brief Calculate total bonus for a specific stat from all upgrades.
     * @param statName Name of the stat to calculate total bonus for.
     * @return Total bonus value for the stat.
     */
    float getTotalStatBonus(const std::string& statName) const;

    /**
     * @brief Get a TowerStat object with all cumulative bonuses from upgrades.
     * @return TowerStat object containing all upgrade bonuses.
     */
    EntityStat getAllUpgradeBonuses() const;

    /**
     * @brief Check if an upgrade type can be upgraded.
     * @param typeId The upgrade type ID.
     * @param playerCurrency Player's current currency.
     * @return True if the upgrade is possible.
     */
    bool canUpgrade(int typeId, const Currency& playerCurrency) const;

    /**
     * @brief Get the cost for the next level of an upgrade type.
     * @param typeId The upgrade type ID.
     * @return Pointer to upgrade details for next level, or nullptr if not possible.
     */
    const UpgradeDetails* getNextUpgradeDetail(int typeId) const;

    /**
     * @brief Check if any upgrade type has reached its evolution condition.
     * @return Vector of tower types that this tower can evolve to.
     */
    std::vector<std::string> getAvailableEvolutions() const;

    /**
     * @brief Get the total number of upgrades purchased.
     * @return Current total upgrade count.
     */
    int getTotalUpgradeCount() const { return totalUpgradeCount; }

    /**
     * @brief Get the maximum total upgrades allowed.
     * @return Maximum total upgrades.
     */
    int getMaxTotalUpgrades() const { return maxTotalUpgrades; }

    /**
     * @brief Check if the total upgrade limit has been reached.
     * @return True if no more upgrades are allowed.
     */
    bool isTotalUpgradeLimitReached() const {
        return totalUpgradeCount >= maxTotalUpgrades;
    }

    /**
     * @brief Reset all upgrades (for testing or tower reset).
     */
    void resetAllUpgrades();

private:
    /**
     * @brief Apply an upgrade level (internal method).
     * @param typeId The upgrade type ID.
     * @param newLevel The new level to set.
     */
    void applyUpgrade(int typeId, int newLevel);
};
