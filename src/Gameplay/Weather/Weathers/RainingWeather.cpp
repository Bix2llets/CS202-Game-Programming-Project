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

RainingWeather::RainingWeather(Level& level) : Weather(WeatherType::Raining, "raining", level) {
    // Load rain overlay texture
    loadTexture(id);
    
    float size = std::max(level.getMapSize().x, level.getMapSize().y);
    Logger::debug("RainingWeather: Size " + std::to_string(size));
    overlayAnimation.updateSpriteSize(size, size);
    overlayAnimation.setPosition({0, 0});
    
    Logger::debug("RainingWeather: Created rainy weather with effects");
}

void RainingWeather::update() {
    Weather::update();
    // Update rain animation or particle effects
    overlayAnimation.update();
    overlaySprite = overlayAnimation.getCurrentSprite();
    overlaySprite.setColor(sf::Color(255, 255, 255, (int) (overlayOpacity * 255)));
}

void RainingWeather::applyToTower(Tower* tower) {
    Weather::applyToTower(tower);
}

void RainingWeather::applyToEnemy(Enemy* enemy) {
    try {
        EntityStat stats = enemy->getStats();
        
    // Reduce enemy speed by 20%
        float currentSpeed = stats.getStat("speed");
        float reducedSpeed = currentSpeed * (1.0f - ENEMY_SPEED_REDUCTION);
        enemy->setStat("speed", reducedSpeed);
        
        // Reduce burn damage and duration if the enemy has burn effects
        if (stats.hasStat("effect_burn_level")) {
            float currentBurnDamage = stats.getStat("effect_burn_level");
            float reducedBurnDamage = currentBurnDamage * (1.0f - BURN_DAMAGE_REDUCTION);
            enemy->setStat("effect_burn_level", reducedBurnDamage);
        }
        
        if (stats.hasStat("effect_burn_duration")) {
            float currentBurnDuration = stats.getStat("effect_burn_duration");
            float reducedBurnDuration = currentBurnDuration * (1.0f - BURN_DURATION_REDUCTION);
            enemy->setStat("effect_burn_duration", reducedBurnDuration);
        }
        
        // Logger::debug("RainingWeather: Applied rain effects to enemy " + std::to_string(enemy->getId()) + 
        //              " - Speed reduced from " + std::to_string(currentSpeed) + 
        //              " to " + std::to_string(reducedSpeed));
    } catch (const std::exception& e) {
        Logger::error("RainingWeather: Failed to apply effects to enemy: " + std::string(e.what()));
    }
}

void RainingWeather::removeFromTower(Tower* tower) {
    Weather::removeFromTower(tower);
}

void RainingWeather::removeFromEnemy(Enemy* enemy) {
   
}
