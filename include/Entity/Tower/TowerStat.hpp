#pragma once

#include <unordered_map>
#include <string>

class TowerStat {
private:
    std::unordered_map<std::string, float> stats;

public:
    // Constructor
    TowerStat() = default;
    
    // Destructor (non-virtual since class won't be inherited)
    ~TowerStat() = default;

    // Static string constants
    static inline const std::string RANGE = "RANGE";
    static inline const std::string DAMAGE = "DAMAGE";
    static inline const std::string SPEED = "FIRE_RATE";
    static inline const std::string MAX_TARGETS = "MAX_TARGETS";

    static inline const std::string DAMAGE_RADIUS = "DAMAGE_RADIUS";

    // For Flamethrowers
    static inline const std::string BURN_DAMAGE = "BURN_DAMAGE";
    static inline const std::string BURN_DURATION = "BURN_DURATION";
    static inline const std::string CHARGE = "CHARGE";
    static inline const std::string CHARGE_UP_RATE = "CHARGE_UP_RATE";
    static inline const std::string CHARGE_DOWN_RATE = "CHARGE_DOWN_RATE";
    static inline const std::string CHARGE_THRESHOLD = "CHARGE_THRESHOLD";

    // For Machine Guns
    static inline const std::string ACCURACY_RATE = "ACCURACY_RATE";

    // For Rocket Launchers
    static inline const std::string ROCKET_SPEED = "ROCKET_SPEED";
    static inline const std::string ROCKET_COUNT = "ROCKET_COUNT";

    static inline std::string multiplier(const std::string& statName) {
        return statName + "_MULTIPLIER";
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
};