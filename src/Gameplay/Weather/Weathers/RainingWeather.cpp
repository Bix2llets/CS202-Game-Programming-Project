/**
 * @file RainingWeather.cpp
 * @brief Implementation of the RainingWeather class.
 */

#include "Gameplay/Weather/Weathers/RainingWeather.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Modules/EntityStat.hpp"
#include "Scene/Level.hpp"
#include "Utility/logger.hpp"

RainingWeather::RainingWeather(Level& level) : Weather(WeatherType::Raining, level) {
    // Load rain overlay texture
    loadOverlay("raining");
    overlaySprite.setPosition({0, 0});

    float size = std::max(level.getMapSize().x, level.getMapSize().y);
    overlayAnimation.updateSpriteSize(size, size);

    Logger::debug("RainingWeather: Created rainy weather with effects");
}

void RainingWeather::update() {
    // Update rain animation or particle effects
    overlayAnimation.update();
    overlaySprite = overlayAnimation.getCurrentSprite();
}

void RainingWeather::applyToTower(Tower* tower) {
    try {
        EntityStat* stats = tower->getStats();
        stats->addStat(EntityStat::multiplier("range"), -TOWER_RANGE_REDUCTION);
        
        Logger::debug("RainingWeather: Applied rain effects to tower " + std::to_string(tower->getUniqueId()) + 
                      " - Range reduced by " + std::to_string(TOWER_RANGE_REDUCTION * 100) + "%");
    } catch (const std::exception& e) {
        Logger::error("RainingWeather: Failed to apply effects to tower: " + std::string(e.what()));
    }
}

void RainingWeather::applyToEnemy(Enemy* enemy) {
    // try {
    //     EntityStat& stats = enemy.getStats();
        
    //     // Reduce enemy speed by 20%
    //     float currentSpeed = stats.getStat("speed");
    //     float reducedSpeed = currentSpeed * (1.0f - ENEMY_SPEED_REDUCTION);
    //     stats.setStat("speed", reducedSpeed);
        
    //     // Reduce burn damage and duration if the enemy has burn effects
    //     if (stats.hasStat("effect_burn_level")) {
    //         float currentBurnDamage = stats.getStat("effect_burn_level");
    //         float reducedBurnDamage = currentBurnDamage * (1.0f - BURN_DAMAGE_REDUCTION);
    //         stats.setStat("effect_burn_level", reducedBurnDamage);
    //     }
        
    //     if (stats.hasStat("effect_burn_duration")) {
    //         float currentBurnDuration = stats.getStat("effect_burn_duration");
    //         float reducedBurnDuration = currentBurnDuration * (1.0f - BURN_DURATION_REDUCTION);
    //         stats.setStat("effect_burn_duration", reducedBurnDuration);
    //     }
        
    //     Logger::debug("RainingWeather: Applied rain effects to enemy " + std::to_string(enemy.getId()) + 
    //                  " - Speed reduced from " + std::to_string(currentSpeed) + 
    //                  " to " + std::to_string(reducedSpeed));
    // } catch (const std::exception& e) {
    //     Logger::error("RainingWeather: Failed to apply effects to enemy: " + std::string(e.what()));
    // }
}

void RainingWeather::removeFromTower(Tower* tower) {
    try {
        EntityStat* stats = tower->getStats();
        stats->addStat(EntityStat::multiplier("range"), TOWER_RANGE_REDUCTION);
        
        Logger::debug("RainingWeather: Removed rain effects from tower " + std::to_string(tower->getUniqueId()) + 
                      " - Range restored by " + std::to_string(TOWER_RANGE_REDUCTION * 100) + "%");
    } catch (const std::exception& e) {
        Logger::error("RainingWeather: Failed to apply effects to tower: " + std::string(e.what()));
    }
}

void RainingWeather::removeFromEnemy(Enemy* enemy) {
    // try {
    //     EntityStat& stats = enemy.getStats();
        
    //     // Restore enemy speed by reversing the reduction
    //     float currentSpeed = stats.getStat("speed");
    //     float originalSpeed = currentSpeed / (1.0f - ENEMY_SPEED_REDUCTION);
    //     stats.setStat("speed", originalSpeed);
        
    //     // Restore burn effects if they exist
    //     if (stats.hasStat("effect_burn_level")) {
    //         float currentBurnDamage = stats.getStat("effect_burn_level");
    //         float originalBurnDamage = currentBurnDamage / (1.0f - BURN_DAMAGE_REDUCTION);
    //         stats.setStat("effect_burn_level", originalBurnDamage);
    //     }
        
    //     if (stats.hasStat("effect_burn_duration")) {
    //         float currentBurnDuration = stats.getStat("effect_burn_duration");
    //         float originalBurnDuration = currentBurnDuration / (1.0f - BURN_DURATION_REDUCTION);
    //         stats.setStat("effect_burn_duration", originalBurnDuration);
    //     }
        
    //     Logger::debug("RainingWeather: Removed rain effects from enemy " + std::to_string(enemy.getId()) + 
    //                  " - Speed restored from " + std::to_string(currentSpeed) + 
    //                  " to " + std::to_string(originalSpeed));
    // } catch (const std::exception& e) {
    //     Logger::error("RainingWeather: Failed to remove effects from enemy: " + std::string(e.what()));
    // }
}
