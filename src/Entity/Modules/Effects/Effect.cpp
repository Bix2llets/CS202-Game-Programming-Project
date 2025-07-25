/**
 * @file Effect.cpp
 * @brief Implementation of the Effect class.
 */

#include "Entity/Modules/Effects/Effect.hpp"

Effect::Effect(EffectType type, EffectID id, int level, float duration) 
    : type(type), id(id), level(level) {
    
    // Set up duration timer (Single mode for effect expiration)
    timer = Timer(duration, TimerMode::Single);
    
    // Set up tick timer for periodic effects (0.5 seconds)
    if (isPeriodic()) {
        tickTimer = Timer(0.5f, TimerMode::Continuous);
    }
}

void Effect::update() {
    timer.update();
    if (isPeriodic()) {
        tickTimer.update();
    }
}

bool Effect::isExpired() const {
    return timer.isAvailable();
}

bool Effect::shouldTick() const {
    return isPeriodic() && tickTimer.isAvailable();
}

void Effect::resetTickTimer() {
    if (isPeriodic()) {
        tickTimer.use();
    }
}

EffectID Effect::getID() const {
    return id;
}

EffectType Effect::getType() const {
    return type;
}

int Effect::getLevel() const {
    return level;
}

float Effect::getDuration() const {
    return timer.getInterval();
}

bool Effect::isPeriodic() const {
    return type == EffectType::Burn || 
           type == EffectType::NapalmBurn || 
           type == EffectType::Regeneration;
}

void Effect::refresh(int newLevel, float newDuration) {
    level = newLevel;
    // Reset duration timer but keep tick timer state
    timer = Timer(newDuration, TimerMode::Single);
    // tickTimer remains unchanged - preserves tick timing!
}
