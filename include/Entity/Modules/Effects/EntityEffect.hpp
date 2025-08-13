/**
 * @file EntityEffect.hpp
 * @brief Declares the EntityEffect class for managing multiple effects on an entity.
 *
 * EntityEffect manages a collection of active effects and provides methods to
 * calculate cumulative modifiers and handle effect interactions. Effects with
 * the same ID will override each other while preserving tick timing.
 */
#pragma once

#include <memory>
#include <vector>

#include "Effect.hpp"

/**
 * @class EntityEffect
 * @brief Manages multiple status effects on an entity.
 *
 * This class handles adding, removing, and updating effects, as well as
 * calculating cumulative modifiers from all active effects. Effects with
 * the same ID and EffectType will override each other while preserving tick timing.
 */
class EntityEffect {
private:
    std::vector<std::unique_ptr<Effect>> activeEffects;  ///< Collection of active effects

public:
    /**
     * @brief Default constructor.
     */
    EntityEffect() = default;

    /**
     * @brief Destructor.
     */
    ~EntityEffect() = default;

    /**
     * @brief Copy constructor (deep copy).
     * @param other EntityEffect to copy from.
     */
    EntityEffect(const EntityEffect& other);

    /**
     * @brief Copy assignment operator (deep copy).
     * @param other EntityEffect to copy from.
     * @return EntityEffect& Reference to this object.
     */
    EntityEffect& operator=(const EntityEffect& other);

    /**
     * @brief Add a new effect to the entity.
     * If an effect with the same ID and EffectType already exists, it will be refreshed
     * with the new level and duration while preserving tick timing.
     * Otherwise, the effect will be added to the end of the activeEffects list.
     * @param effect Effect to add (ownership is transferred).
     */
    void addEffect(std::unique_ptr<Effect> effect);

    /**
     * @brief Remove an effect by its unique ID.
     * @param id ID of the effect to remove.
     */
    void removeEffect(EffectID id);

    /**
     * @brief Remove all effects of the specified type.
     * @param type Type of effect to remove.
     */
    void removeEffectsByType(EffectType type);

    /**
     * @brief Update all active effects.
     * Removes expired effects and updates timers.
     */
    void update();

    /**
     * @brief Calculate the cumulative damage modifier from all effects.
     * @return float Damage multiplier (1.0 = normal, >1.0 = more damage, <1.0 = less damage).
     */
    float getDamageModifier() const;

    /**
     * @brief Calculate the cumulative speed modifier from all effects.
     * @return float Speed multiplier (1.0 = normal, >1.0 = faster, <1.0 = slower).
     */
    float getSpeedModifier() const;

    /**
     * @brief Get the fire resistance percentage from effects.
     * @return float Fire resistance as a value between 0.0 and 1.0.
     */
    float getFireResistance() const;

    /**
     * @brief Calculate the total burn damage per tick from all Burn effects.
     * Burn effects stack by summation.
     * @return float Total burn damage per tick.
     */
    float getBurnDamage() const;

    /**
     * @brief Calculate the total napalm burn damage per tick from all NapalmBurn effects.
     * NapalmBurn effects stack by summation.
     * @return float Total napalm burn damage per tick.
     */
    float getNapalmBurnDamage() const;

    /**
     * @brief Calculate the total regeneration heal per tick from all Regeneration effects.
     * Regeneration effects stack by summation.
     * @return float Total regeneration heal per tick.
     */
    float getRegenerationHeal() const;

    /**
     * @brief Check if the entity has an effect with a specific ID.
     * @param id Effect ID to check for.
     * @return bool True if the effect is active.
     */
    bool hasEffect(EffectID id) const;

    /**
     * @brief Check if the entity has any effect of a specific type.
     * @param type Effect type to check for.
     * @return bool True if any effect of this type is active.
     */
    bool hasEffectType(EffectType type) const;

    /**
     * @brief Get a specific effect by ID.
     * @param id Effect ID to find.
     * @return Effect* Pointer to the effect, or nullptr if not found.
     */
    Effect* getEffect(EffectID id) const;

    /**
     * @brief Get a specific effect by type.
     * @param type Effect type to find.
     * @return Effect* Pointer to the effect, or nullptr if not found.
     */
    Effect* getEffectByType(EffectType type) const;

    /**
     * @brief Get all effects that have periodic behavior (ticking effects).
     * @return std::vector<Effect*> Vector of pointers to ticking effects.
     */
    std::vector<Effect*> getTickingEffects() const;

    /**
     * @brief Get the number of active effects.
     * @return size_t Number of effects currently active.
     */
    size_t getEffectCount() const;

    /**
     * @brief Clear all active effects.
     */
    void clearAllEffects();
};
