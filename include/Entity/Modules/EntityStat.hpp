#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <json.hpp>

namespace GeneralStat {
    static inline const std::string WIDTH = "texture_width";
    static inline const std::string HEIGHT = "texture_height";
}

namespace EffectStat {
    static inline const std::string BURN_LEVEL = "effect_burn_level";
    static inline const std::string BURN_DURATION = "effect_burn_duration";
    static inline const std::string NAPALM_BURN_LEVEL = "effect_napalm_burn_level";
    static inline const std::string NAPALM_BURN_DURATION = "effect_napalm_burn_duration";
    static inline const std::string REGENERATION_LEVEL = "effect_regeneration_level";
    static inline const std::string REGENERATION_DURATION = "effect_regeneration_duration";
    static inline const std::string VULNERABLE_LEVEL = "effect_vulnerable_level";
    static inline const std::string VULNERABLE_DURATION = "effect_vulnerable_duration";
    static inline const std::string RESISTANCE_LEVEL = "effect_resistance_level";
    static inline const std::string RESISTANCE_DURATION = "effect_resistance_duration";
    static inline const std::string FIRE_RESISTANCE_LEVEL = "effect_fire_resistance_level";
    static inline const std::string FIRE_RESISTANCE_DURATION = "effect_fire_resistance_duration";
    static inline const std::string SLOW_LEVEL = "effect_slow_level";
    static inline const std::string SLOW_DURATION = "effect_slow_duration";
    static inline const std::string ENERGIZED_LEVEL = "effect_energized_level";
    static inline const std::string ENERGIZED_DURATION = "effect_energized_duration";
}

namespace AreaEffectStat {
    static inline const std::string RADIUS = "radius";
    static inline const std::string DAMAGE = "damage";
    static inline const std::string DURATION = "duration";
    static inline const std::string REPEAT_INTERVAL = "repeating_per_interval";
    static inline const std::string REPEAT_TIMES = "repeating_times";
}

namespace TowerStat {
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
}

namespace EnemyStat {
    static inline const std::string MAX_HEALTH = "max_health";
    static inline const std::string SPEED = "speed";

    static inline const std::string RESISTANCE = "resistance";
    static inline const std::string FIRE_RESISTANCE = "fire_resistance";
    static inline const std::string REGENERATION = "heal_amount";
}

class EntityStat {
private:
    std::unordered_map<std::string, float> stats;

public:
    // Constructor
    EntityStat() = default;
    
    // Copy constructor
    EntityStat(const EntityStat& other) = default;
    
    // Assignment operator
    EntityStat& operator=(const EntityStat& other) = default;
    
    // Destructor (non-virtual since class won't be inherited)
    ~EntityStat() = default;

    static inline std::string multiplier(const std::string& statName) {
        return statName + "_multiplier";
    }
    
    float getStat(const std::string& statName, float defaultValue = 0.0f) const {
        float stat = getStatWithoutBonus(statName, defaultValue);
        float statMultiplier = 1.0f + getStatWithoutBonus(multiplier(statName), 0.0f);
        return stat * statMultiplier;
    }

    float getStatWithoutBonus(const std::string& statName, float defaultValue = 0.0f) const {
        auto it = stats.find(statName);
        if (it != stats.end()) {
            return it->second;
        }
        return defaultValue;
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

    void loadFromJson(const nlohmann::json& statsJson) {
        // Parse all stats from the JSON
        for (auto it = statsJson.begin(); it != statsJson.end(); ++it) {
            const std::string& statName = it.key();
            float statValue = it.value().get<float>();

            setStat(statName, statValue);
        }
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
     * @brief Add another EntityStat's values to this one.
     * @param other The EntityStat to add.
     * @return Reference to this EntityStat.
     */
    EntityStat& operator+=(const EntityStat& other) {
        for (const auto& [statName, value] : other.stats) {
            addStat(statName, value);
        }
        return *this;
    }

    /**
     * @brief Subtract another EntityStat's values from this one.
     * @param other The EntityStat to subtract.
     * @return Reference to this EntityStat.
     */
    EntityStat& operator-=(const EntityStat& other) {
        for (const auto& [statName, value] : other.stats) {
            addStat(statName, -value);
        }
        return *this;
    }

    /**
     * @brief Add two EntityStat objects.
     * @param other The EntityStat to add.
     * @return New EntityStat with combined values.
     */
    EntityStat operator+(const EntityStat& other) const {
        EntityStat result = *this;
        result += other;
        return result;
    }

    /**
     * @brief Subtract two EntityStat objects.
     * @param other The EntityStat to subtract.
     * @return New EntityStat with subtracted values.
     */
    EntityStat operator-(const EntityStat& other) const {
        EntityStat result = *this;
        result -= other;
        return result;
    }

    /**
     * @brief Multiply two EntityStat objects.
     * @param other The EntityStat to multiply with.
     * @return New EntityStat with multiplied values.
     */
    EntityStat operator*(const EntityStat& other) const {
        EntityStat result = *this;
        for (const auto& [statName, value] : other.stats) {
            if (hasStat(statName)) {
                result.setStat(statName, getStatWithoutBonus(statName) * value);
            }
        }
        return result;
    }

    EntityStat& operator*=(const EntityStat& other) {
        for (const auto& [statName, value] : other.stats) {
            if (hasStat(statName)) {
                setStat(statName, getStatWithoutBonus(statName) * value);
            }
        }
        return *this;
    }

    /**
     * @brief Equality operator.
     * @param other The EntityStat to compare with.
     * @return True if both EntityStat objects have the same stats.
     */
    bool operator==(const EntityStat& other) const {
        return stats == other.stats;
    }

    /**
     * @brief Inequality operator.
     * @param other The EntityStat to compare with.
     * @return True if EntityStat objects have different stats.
     */
    bool operator!=(const EntityStat& other) const {
        return !(*this == other);
    }
};