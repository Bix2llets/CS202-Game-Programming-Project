/**
 * @file UpgradeDetails.hpp
 * @brief Defines the UpgradeDetails struct for storing upgrade cost and bonus stats.
 */

#pragma once

#include <unordered_map>
#include <string>
#include "Gameplay/Currency.hpp"
#include "Entity/Tower/TowerStat.hpp"

/**
 * @struct UpgradeDetails
 * @brief Contains the cost and bonus stats for a specific upgrade level.
 * 
 * This struct represents what's needed to perform an upgrade (cost) and 
 * what benefits are gained (bonus stats).
 */
struct UpgradeDetails {
    Currency cost;        ///< Cost in game currencies (scrap, petroleum)
    TowerStat bonusStats; ///< Bonus stats provided by this upgrade level

    /**
     * @brief Default constructor.
     */
    UpgradeDetails() = default;

    /**
     * @brief Constructor with cost and bonus stats.
     * @param upgradeCost The currency cost for this upgrade level.
     * @param stats The bonus stats provided by this upgrade level.
     */
    UpgradeDetails(const Currency& upgradeCost, const TowerStat& stats)
        : cost(upgradeCost), bonusStats(stats) {}

    /**
     * @brief Get bonus value for a specific stat.
     * @param statName Name of the stat to retrieve.
     * @param defaultValue Default value if stat is not found.
     * @return The bonus value for the specified stat.
     */
    float getBonusStat(const std::string& statName, float defaultValue = 0.0f) const {
        return bonusStats.getStat(statName, defaultValue);
    }

    /**
     * @brief Check if this upgrade provides a bonus for the specified stat.
     * @param statName Name of the stat to check.
     * @return True if the stat has a bonus value.
     */
    bool hasBonusStat(const std::string& statName) const {
        return bonusStats.hasStat(statName);
    }

    /**
     * @brief Get the bonus stats object.
     * @return Reference to the bonus stats TowerStat object.
     */
    const TowerStat& getBonusStats() const {
        return bonusStats;
    }

    /**
     * @brief Get the bonus stats object (non-const).
     * @return Reference to the bonus stats TowerStat object.
     */
    TowerStat& getBonusStats() {
        return bonusStats;
    }
};
