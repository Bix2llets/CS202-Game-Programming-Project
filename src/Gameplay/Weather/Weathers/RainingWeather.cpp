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
    loadOverlay(id);
    
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
}

void RainingWeather::removeFromTower(Tower* tower) {
    Weather::removeFromTower(tower);
}

void RainingWeather::removeFromEnemy(Enemy* enemy) {
   
}
