/**
 * @file EntityEffect.cpp
 * @brief Implementation of the EntityEffect class.
 */

#include "Entity/Modules/Effects/EntityEffect.hpp"
#include <algorithm>
#include <cmath>

EntityEffect::EntityEffect(const EntityEffect& other) {
    activeEffects.reserve(other.activeEffects.size());
    effectIDMap.clear();
    
    for (size_t i = 0; i < other.activeEffects.size(); ++i) {
        const auto& effect = other.activeEffects[i];
        auto newEffect = std::make_unique<Effect>(
            effect->getType(), 
            effect->getID(), 
            effect->getLevel(), 
            effect->getDuration()
        );
        effectIDMap[newEffect->getID()] = i;
        activeEffects.push_back(std::move(newEffect));
    }
}

EntityEffect& EntityEffect::operator=(const EntityEffect& other) {
    if (this != &other) {
        activeEffects.clear();
        effectIDMap.clear();
        
        activeEffects.reserve(other.activeEffects.size());
        for (size_t i = 0; i < other.activeEffects.size(); ++i) {
            const auto& effect = other.activeEffects[i];
            auto newEffect = std::make_unique<Effect>(
                effect->getType(), 
                effect->getID(), 
                effect->getLevel(), 
                effect->getDuration()
            );
            effectIDMap[newEffect->getID()] = i;
            activeEffects.push_back(std::move(newEffect));
        }
    }
    return *this;
}

void EntityEffect::addEffect(std::unique_ptr<Effect> effect) {
    EffectID id = effect->getID();
    
    // Check if effect with same ID already exists
    auto it = effectIDMap.find(id);
    if (it != effectIDMap.end()) {
        // Refresh existing effect (preserves tick timer)
        size_t index = it->second;
        activeEffects[index]->refresh(effect->getLevel(), effect->getDuration());
    } else {
        // Add new effect
        size_t index = activeEffects.size();
        effectIDMap[id] = index;
        activeEffects.push_back(std::move(effect));
    }
}

void EntityEffect::removeEffect(EffectID id) {
    auto it = effectIDMap.find(id);
    if (it != effectIDMap.end()) {
        size_t index = it->second;
        
        // Remove from vector
        activeEffects.erase(activeEffects.begin() + index);
        effectIDMap.erase(it);
        
        // Update indices in map for effects after the removed one
        for (auto& pair : effectIDMap) {
            if (pair.second > index) {
                pair.second--;
            }
        }
    }
}

void EntityEffect::removeEffectsByType(EffectType type) {
    // Collect IDs to remove to avoid iterator invalidation
    std::vector<EffectID> idsToRemove;
    for (const auto& effect : activeEffects) {
        if (effect->getType() == type) {
            idsToRemove.push_back(effect->getID());
        }
    }
    
    // Remove all effects of this type
    for (EffectID id : idsToRemove) {
        removeEffect(id);
    }
}

void EntityEffect::update() {
    // Update all effects and collect expired ones
    std::vector<EffectID> expiredIDs;
    
    for (const auto& effect : activeEffects) {
        effect->update();
        if (effect->isExpired()) {
            expiredIDs.push_back(effect->getID());
        }
    }
    
    // Remove expired effects
    for (EffectID id : expiredIDs) {
        removeEffect(id);
    }
}

float EntityEffect::getDamageModifier() const {
    int vulnerableModifier = 0;  // Maximum vulnerable effect
    int resistanceModifier = 0;  // Maximum resistance effect
    
    for (const auto& effect : activeEffects) {
        switch (effect->getType()) {
            case EffectType::Vulnerable:
                vulnerableModifier = std::max(vulnerableModifier, effect->getLevel());
                break;
            case EffectType::Resistance:
                resistanceModifier = std::max(resistanceModifier, effect->getLevel());
                break;
            default:
                break;
        }
    }
    
    // Apply modifiers: vulnerable increases damage taken, resistance reduces it
    float modifier = 1.0f + (vulnerableModifier - resistanceModifier) / 100.0f;
    return std::max(0.0f, modifier);  // Ensure modifier doesn't go negative
}

float EntityEffect::getSpeedModifier() const {
    int slowModifier = 0;      // Maximum slow effect  
    int energizedModifier = 0; // Maximum energized effect
    
    for (const auto& effect : activeEffects) {
        switch (effect->getType()) {
            case EffectType::Slow:
                slowModifier = std::max(slowModifier, effect->getLevel());
                break;
            case EffectType::Energized:
                energizedModifier = std::max(energizedModifier, effect->getLevel());
                break;
            default:
                break;
        }
    }
    
    // Apply modifiers: slow reduces speed, energized increases it
    float modifier = 1.0f + (energizedModifier - slowModifier) / 100.0f;
    return std::max(0.0f, modifier);  // Ensure modifier doesn't go negative
}

float EntityEffect::getFireResistance() const {
    int maxResistance = 0;  // Maximum fire resistance
    
    for (const auto& effect : activeEffects) {
        if (effect->getType() == EffectType::FireResistance) {
            maxResistance = std::max(maxResistance, effect->getLevel());
        }
    }

    return std::min(maxResistance / 100.0f, 1.0f);  // Cap at 100% resistance
}

float EntityEffect::getBurnDamage() const {
    float totalDamage = 0.0f;  // Sum all burn effects
    
    for (const auto& effect : activeEffects) {
        if (effect->getType() == EffectType::Burn) {
            totalDamage += effect->getLevel();
        }
    }
    
    return totalDamage;
}

float EntityEffect::getNapalmBurnDamage() const {
    float totalDamage = 0.0f;  // Sum all napalm burn effects
    
    for (const auto& effect : activeEffects) {
        if (effect->getType() == EffectType::NapalmBurn) {
            totalDamage += effect->getLevel();
        }
    }
    
    return totalDamage;
}

float EntityEffect::getRegenerationHeal() const {
    float totalHeal = 0.0f;  // Sum all regeneration effects
    
    for (const auto& effect : activeEffects) {
        if (effect->getType() == EffectType::Regeneration) {
            totalHeal += effect->getLevel();
        }
    }
    
    return totalHeal;
}

bool EntityEffect::hasEffect(EffectID id) const {
    return effectIDMap.find(id) != effectIDMap.end();
}

bool EntityEffect::hasEffectType(EffectType type) const {
    for (const auto& effect : activeEffects) {
        if (effect->getType() == type) {
            return true;
        }
    }
    return false;
}

Effect* EntityEffect::getEffect(EffectID id) const {
    auto it = effectIDMap.find(id);
    if (it != effectIDMap.end()) {
        return activeEffects[it->second].get();
    }
    return nullptr;
}

Effect* EntityEffect::getEffectByType(EffectType type) const {
    for (const auto& effect : activeEffects) {
        if (effect->getType() == type) {
            return effect.get();
        }
    }
    return nullptr;
}

std::vector<Effect*> EntityEffect::getTickingEffects() const {
    std::vector<Effect*> tickingEffects;
    
    for (const auto& effect : activeEffects) {
        if (effect->isPeriodic()) {
            tickingEffects.push_back(effect.get());
        }
    }
    
    return tickingEffects;
}

size_t EntityEffect::getEffectCount() const {
    return activeEffects.size();
}

void EntityEffect::clearAllEffects() {
    activeEffects.clear();
    effectIDMap.clear();
}
