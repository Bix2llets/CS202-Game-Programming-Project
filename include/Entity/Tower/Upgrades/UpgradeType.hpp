/**
 * @file UpgradeType.hpp
 * @brief Defines the UpgradeType class for managing a specific type of tower upgrade.
 */

#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "UpgradeDetails.hpp"

/**
 * @class UpgradeType
 * @brief Represents a single upgrade type (e.g., "Rapid Fire", "Extended Range").
 * 
 * Each upgrade type has multiple levels, each with its own cost and bonus stats.
 * This class manages all the data for one upgrade type as defined in the JSON.
 */
class UpgradeType {
private:
    std::string displayName;        ///< User-friendly name for the upgrade
    std::string description;        ///< Description with placeholders for bonus values
    int maxLevel;                   ///< Maximum level for this upgrade type
    std::string iconPath;           ///< Path to the upgrade icon
    std::string evolveTo;           ///< Tower type to evolve to at max level
    std::unordered_map<int, UpgradeDetails> levelDetails; ///< Map of level -> upgrade details

public:
    /**
     * @brief Default constructor.
     */
    UpgradeType() = default;

    /**
     * @brief Constructor with basic upgrade information.
     * @param name Display name for the upgrade.
     * @param desc Description of the upgrade.
     * @param maxLvl Maximum level for this upgrade.
     * @param icon Path to the upgrade icon.
     * @param evolve Tower type to evolve to.
     */
    UpgradeType(const std::string& name, const std::string& desc, int maxLvl, 
                const std::string& icon, const std::string& evolve)
        : displayName(name), description(desc), maxLevel(maxLvl), 
          iconPath(icon), evolveTo(evolve) {}

    /**
     * @brief Add upgrade details for a specific level.
     * @param level The upgrade level (1, 2, 3, etc.).
     * @param details The upgrade details (cost and bonus stats).
     */
    void addLevelDetails(int level, const UpgradeDetails& details) {
        if (level >= 1 && level <= maxLevel) {
            levelDetails[level] = details;
        }
    }

    /**
     * @brief Get upgrade details for a specific level.
     * @param level The upgrade level to retrieve.
     * @return Pointer to upgrade details, or nullptr if level is invalid.
     */
    const UpgradeDetails* getLevelDetails(int level) const {
        auto it = levelDetails.find(level);
        return it != levelDetails.end() ? &it->second : nullptr;
    }

    /**
     * @brief Check if a level is valid for this upgrade type.
     * @param level The level to check.
     * @return True if the level is valid and has details.
     */
    bool isValidLevel(int level) const {
        return level >= 1 && level <= maxLevel && levelDetails.find(level) != levelDetails.end();
    }

    // Getters
    const std::string& getDisplayName() const { return displayName; }
    const std::string& getDescription() const { return description; }
    int getMaxLevel() const { return maxLevel; }
    const std::string& getIconPath() const { return iconPath; }
    const std::string& getEvolveTo() const { return evolveTo; }

    /**
     * @brief Get the total bonus for a stat across all levels up to the specified level.
     * @param statName Name of the stat to calculate total bonus for.
     * @param currentLevel Current level of this upgrade type.
     * @return Total bonus value for the stat.
     */
    float getTotalBonusForStat(const std::string& statName, int currentLevel) const {
        float total = 0.0f;
        for (int i = 1; i <= currentLevel && i <= maxLevel; ++i) {
            auto it = levelDetails.find(i);
            if (it != levelDetails.end()) {
                total += it->second.getBonusStat(statName, 0.0f);
            }
        }
        return total;
    }

    /**
     * @brief Get all upgrade levels and their details.
     * @return Reference to the level details map.
     */
    const std::unordered_map<int, UpgradeDetails>& getAllLevelDetails() const {
        return levelDetails;
    }

    // Setters (for JSON loading)
    void setDisplayName(const std::string& name) { displayName = name; }
    void setDescription(const std::string& desc) { description = desc; }
    void setMaxLevel(int maxLvl) { maxLevel = maxLvl; }
    void setIconPath(const std::string& icon) { iconPath = icon; }
    void setEvolveTo(const std::string& evolve) { evolveTo = evolve; }
};
