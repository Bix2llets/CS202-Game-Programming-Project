/**
 * @file SunnyWeather.cpp
 * @brief Implementation of the SunnyWeather class.
 */

#include "Gameplay/Weather/Weathers/SunnyWeather.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Scene/Level.hpp"
#include "Utility/logger.hpp"

SunnyWeather::SunnyWeather(Level& level) : Weather(WeatherType::Sunny, "sunny", level) {
    // No overlay for sunny weather
    overlayActive = false;
    Logger::debug("SunnyWeather: Created sunny weather (no effects)");
}

void SunnyWeather::update() {
    // Call base class update for opacity handling
    Weather::update();
    // Sunny weather has no special update logic
}

void SunnyWeather::applyToTower(Tower* tower) {
    // Sunny weather has no effects on towers
}

void SunnyWeather::applyToEnemy(Enemy* enemy) {
    // Sunny weather has no effects on enemies
}

void SunnyWeather::removeFromTower(Tower* tower) {
    // No effects to remove for sunny weather
}

void SunnyWeather::removeFromEnemy(Enemy* enemy) {
    // No effects to remove for sunny weather
}
