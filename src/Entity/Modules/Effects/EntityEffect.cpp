/**
 * @file EntityEffect.cpp
 * @brief Implementation of the EntityEffect class.
 */

#include "Entity/Modules/Effects/EntityEffect.hpp"
#include <algorithm>
#include <cmath>

EntityEffect::EntityEffect(const EntityEffect& other) {
    activeEffects.reserve(other.activeEffects.size());
    
    for (const auto& effect : other.activeEffects) {
        auto newEffect = std::make_unique<Effect>(
            effect->getType(), 
            effect->getID(), 
            effect->getLevel(), 
            effect->getDuration()
        );
        activeEffects.push_back(std::move(newEffect));
    }
}

EntityEffect& EntityEffect::operator=(const EntityEffect& other) {
    if (this != &other) {
        activeEffects.clear();
        
        activeEffects.reserve(other.activeEffects.size());
        for (const auto& effect : other.activeEffects) {
            auto newEffect = std::make_unique<Effect>(
                effect->getType(), 
                effect->getID(), 
                effect->getLevel(), 
                effect->getDuration()
            );
            activeEffects.push_back(std::move(newEffect));
        }
    }
    return *this;
}

void EntityEffect::addEffect(std::unique_ptr<Effect> effect) {
    EffectID id = effect->getID();
    EffectType type = effect->getType();
    
    // Check if effect with same ID and EffectType already exists
    for (auto& existingEffect : activeEffects) {
        if (existingEffect->getID() == id && existingEffect->getType() == type) {
            // Refresh existing effect (preserves tick timer)
            existingEffect->refresh(effect->getLevel(), effect->getDuration());
            return;
        }
    }
    
    // Add new effect to the end of the list
    activeEffects.push_back(std::move(effect));
}

void EntityEffect::removeEffect(EffectID id) {
    activeEffects.erase(
        std::remove_if(activeEffects.begin(), activeEffects.end(),
            [id](const std::unique_ptr<Effect>& effect) {
                return effect->getID() == id;
            }),
        activeEffects.end()
    );
}

void EntityEffect::removeEffectsByType(EffectType type) {
    activeEffects.erase(
        std::remove_if(activeEffects.begin(), activeEffects.end(),
            [type](const std::unique_ptr<Effect>& effect) {
                return effect->getType() == type;
            }),
        activeEffects.end()
    );
}

void EntityEffect::update() {
    // Remove expired effects
    activeEffects.erase(
        std::remove_if(activeEffects.begin(), activeEffects.end(),
            [](std::unique_ptr<Effect>& effect) {
                effect->update();
                return effect->isExpired();
            }),
        activeEffects.end()
    );
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
    for (const auto& effect : activeEffects) {
        if (effect->getID() == id) {
            return true;
        }
    }
    return false;
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
    for (const auto& effect : activeEffects) {
        if (effect->getID() == id) {
            return effect.get();
        }
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
}
