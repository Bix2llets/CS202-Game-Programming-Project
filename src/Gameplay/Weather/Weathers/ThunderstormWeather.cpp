/**
 * @file ThunderstormWeather.cpp
 * @brief Implementation of the ThunderstormWeather class.
 */

#include "Gameplay/Weather/Weathers/ThunderstormWeather.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Modules/EntityStat.hpp"
#include "Scene/Level.hpp"
#include "Utility/logger.hpp"

ThunderstormWeather::ThunderstormWeather(Level& level) : Weather(WeatherType::Thunderstorm, "thunderstorm", level) {
    // Load thunderstorm overlay texture
    loadTexture(id);
    
    float size = std::max(level.getMapSize().x, level.getMapSize().y);
    Logger::debug("Thunderstorm: Size " + std::to_string(size));
    overlayAnimation.updateSpriteSize(size, size);
    overlayAnimation.setPosition({0, 0});

    Logger::debug("ThunderstormWeather: Created thunderstorm weather with effects");
}

void ThunderstormWeather::update() {
    Weather::update();
    // Update thunderstorm animation or particle effects
    overlayAnimation.update();
    overlaySprite = overlayAnimation.getCurrentSprite();
    overlaySprite.setColor(sf::Color(255, 255, 255, (int) (overlayOpacity * 255)));
}

void ThunderstormWeather::applyToTower(Tower* tower) {
    Weather::applyToTower(tower);
}

void ThunderstormWeather::applyToEnemy(Enemy* enemy) {
    Weather::applyToEnemy(enemy);
}

void ThunderstormWeather::removeFromTower(Tower* tower) {
    Weather::removeFromTower(tower);
}

void ThunderstormWeather::removeFromEnemy(Enemy* enemy) {
   Weather::removeFromEnemy(enemy);
}
