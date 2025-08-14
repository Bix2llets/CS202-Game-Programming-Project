/**
 * @file Effect.hpp
 * @brief Declares the Effect class for individual status effects on entities.
 *
 * Effects represent temporary status changes that can be applied to entities,
 * such as burn damage, speed modifications, or resistance buffs.
 */
#pragma once

#include <cstdint>
#include "Entity/Modules/Timer.hpp"

/**
 * @enum EffectType
 * @brief Enumeration for different effect types.
 */
enum class EffectType {
    Burn,           ///< Deal fire damage over time (affected by fire resistance) (flat)
    NapalmBurn,     ///< Deal napalm damage over time (ignores fire resistance) (flat)
    Regeneration,   ///< Restore health over time (flat)
    Vulnerable,     ///< Increase damage taken from all sources (percentage)
    Resistance,     ///< Reduce all damage taken from all sources (percentage)
    FireResistance, ///< Reduce fire damage taken (percentage)
    Slow,           ///< Reduce movement speed (percentage)
    Energized       ///< Increase movement speed (percentage)
};

/**
 * @typedef EffectID
 * @brief Unique identifier for effect sources.
 * 
 * Effects with the same ID will override each other while preserving tick timing.
 * Can be generated from tower IDs, spell IDs, or other source identifiers.
 */
using EffectID = std::uint64_t;

/**
 * @class Effect
 * @brief Represents a single status effect that can be applied to an entity.
 *
 * Effects have a type, level (intensity), duration, and unique ID. Some effects like burn
 * and regeneration tick periodically (every 0.5 seconds). Effects with the same ID
 * will override each other while preserving tick timing.
 */
class Effect {
private:
    EffectType type;    ///< Type of effect
    EffectID id;        ///< Unique identifier for this effect source
    int level;          ///< Effect intensity/power
    Timer timer;        ///< Duration timer
    Timer tickTimer;    ///< Timer for periodic effects (0.5s intervals)

public:
    /**
     * @brief Construct a new Effect object.
     * @param type Type of effect to create.
     * @param id Unique identifier for the effect source.
     * @param level Intensity/power of the effect.
     * @param duration Duration of the effect in seconds.
     */
    Effect(EffectType type, int level, float duration, EffectID id);

    /**
     * @brief Update the effect timers.
     * Should be called every frame.
     */
    void update();

    /**
     * @brief Check if the effect has expired.
     * @return bool True if the effect should be removed.
     */
    bool isExpired() const;

    /**
     * @brief Check if the effect should tick (for periodic effects).
     * @return bool True if the effect should apply its periodic effect.
     */
    bool shouldTick() const;

    /**
     * @brief Reset the tick timer after applying a periodic effect.
     */
    void resetTickTimer();

    /**
     * @brief Get the effect ID.
     * @return EffectID The unique identifier of this effect.
     */
    EffectID getID() const;

    /**
     * @brief Get the effect type.
     * @return EffectType The type of this effect.
     */
    EffectType getType() const;

    /**
     * @brief Get the effect level.
     * @return int The intensity/power of this effect.
     */
    int getLevel() const;

    /**
     * @brief Get the effect duration.
     * @return float The duration of this effect in seconds.
     */
    float getDuration() const;

    /**
     * @brief Check if this effect is periodic (ticks every 0.5s).
     * @return bool True if this effect has periodic behavior.
     */
    bool isPeriodic() const;

    /**
     * @brief Refresh this effect with new level and duration.
     * Preserves the tick timer state but resets the duration timer.
     * @param newLevel New intensity/power level.
     * @param newDuration New duration in seconds.
     */
    void refresh(int newLevel, float newDuration);
};
