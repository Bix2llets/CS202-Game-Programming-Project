#pragma once

#include <unordered_map>
#include <string>
#include <vector>

class TowerStat {
private:
    std::unordered_map<std::string, float> stats;

public:
    // Constructor
    TowerStat() = default;
    
    // Copy constructor
    TowerStat(const TowerStat& other) = default;
    
    // Assignment operator
    TowerStat& operator=(const TowerStat& other) = default;
    
    // Destructor (non-virtual since class won't be inherited)
    ~TowerStat() = default;

    // Static string constants
    static inline const std::string RANGE = "range";
    static inline const std::string DAMAGE = "damage";
    static inline const std::string FIRE_RATE = "fire_rate";
    static inline const std::string MAX_TARGETS = "max_targets";

    static inline const std::string DAMAGE_RADIUS = "damage_radius";

    // For Flamethrowers
    static inline const std::string BURN_DAMAGE = "burn_damage";
    static inline const std::string BURN_DURATION = "burn_duration";
    static inline const std::string CHARGE = "charge";
    static inline const std::string CHARGE_UP_RATE = "charge_up_rate";
    static inline const std::string CHARGE_DOWN_RATE = "charge_down_rate";
    static inline const std::string CHARGE_THRESHOLD = "charge_threshold";

    // For Machine Guns
    static inline const std::string ACCURACY_RATE = "accuracy_rate";

    // For Rocket Launchers
    static inline const std::string ROCKET_SPEED = "rocket_speed";
    static inline const std::string ROCKET_COUNT = "rocket_count";

    // For Projectile Towers
    static inline const std::string PROJECTILE_SPEED = "projectile_speed";
    static inline const std::string PROJECTILE_RANGE = "projectile_range";
    static inline const std::string PROJECTILE_PIERCE_COUNT = "projectile_pierce_count";
    static inline const std::string PROJECTILE_STARTING_DISTANCE = "projectile_starting_distance";

    static inline std::string multiplier(const std::string& statName) {
        return statName + "_multiplier";
    }
    
    // static inline std::string flat(const std::string& statName) {
    //     return statName + "_FLAT_BONUS";
    // }
    
    float getStat(const std::string& statName, float defaultValue = 0.0f) const {
        auto it = stats.find(statName);
        if (it != stats.end()) {
            return it->second;
        }
        return defaultValue;
    }

    float getStatWithBonus(const std::string& statName, float defaultValue = 0.0f) const {
        float stat = getStat(statName, defaultValue);
        // float statFlatBonus = getStat(flat(statName), 0.0f);
        float statMultiplier = 1.0f + getStat(multiplier(statName), 0.0f);
        return stat * statMultiplier;
    }

    void setStat(const std::string& statName, float value) {
        stats[statName] = value;
    }

    void addStat(const std::string& statName, float value) {
        stats[statName] += value;
    }

    void multiplyStat(const std::string& statName, float multiplier) {
        stats[statName] *= multiplier;
    }

    /**
     * @brief Check if a stat exists.
     * @param statName Name of the stat to check.
     * @return True if the stat exists, false otherwise.
     */
    bool hasStat(const std::string& statName) const {
        return stats.find(statName) != stats.end();
    }

    /**
     * @brief Remove a stat.
     * @param statName Name of the stat to remove.
     * @return True if the stat was removed, false if it didn't exist.
     */
    bool removeStat(const std::string& statName) {
        return stats.erase(statName) > 0;
    }

    /**
     * @brief Clear all stats.
     */
    void clear() {
        stats.clear();
    }

    /**
     * @brief Check if no stats are set.
     * @return True if the stats map is empty.
     */
    bool isEmpty() const {
        return stats.empty();
    }

    /**
     * @brief Get the number of stats.
     * @return Number of stats currently stored.
     */
    size_t getStatCount() const {
        return stats.size();
    }

    /**
     * @brief Get all stat names.
     * @return Vector of all stat names.
     */
    std::vector<std::string> getStatNames() const {
        std::vector<std::string> names;
        names.reserve(stats.size());
        for (const auto& pair : stats) {
            names.push_back(pair.first);
        }
        return names;
    }

    /**
     * @brief Get direct access to the stats map (const).
     * @return Const reference to the internal stats map.
     */
    const std::unordered_map<std::string, float>& getAllStats() const {
        return stats;
    }

    /**
     * @brief Add another TowerStat's values to this one.
     * @param other The TowerStat to add.
     * @return Reference to this TowerStat.
     */
    TowerStat& operator+=(const TowerStat& other) {
        for (const auto& [statName, value] : other.stats) {
            addStat(statName, value);
        }
        return *this;
    }

    /**
     * @brief Subtract another TowerStat's values from this one.
     * @param other The TowerStat to subtract.
     * @return Reference to this TowerStat.
     */
    TowerStat& operator-=(const TowerStat& other) {
        for (const auto& [statName, value] : other.stats) {
            addStat(statName, -value);
        }
        return *this;
    }

    /**
     * @brief Add two TowerStat objects.
     * @param other The TowerStat to add.
     * @return New TowerStat with combined values.
     */
    TowerStat operator+(const TowerStat& other) const {
        TowerStat result = *this;
        result += other;
        return result;
    }

    /**
     * @brief Subtract two TowerStat objects.
     * @param other The TowerStat to subtract.
     * @return New TowerStat with subtracted values.
     */
    TowerStat operator-(const TowerStat& other) const {
        TowerStat result = *this;
        result -= other;
        return result;
    }

    /**
     * @brief Equality operator.
     * @param other The TowerStat to compare with.
     * @return True if both TowerStat objects have the same stats.
     */
    bool operator==(const TowerStat& other) const {
        return stats == other.stats;
    }

    /**
     * @brief Inequality operator.
     * @param other The TowerStat to compare with.
     * @return True if TowerStat objects have different stats.
     */
    bool operator!=(const TowerStat& other) const {
        return !(*this == other);
    }
};