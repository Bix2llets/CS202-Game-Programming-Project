/**
 * @file Weather.cpp
 * @brief Implementation of the base Weather class.
 */

#include "Gameplay/Weather/Weather.hpp"
#include "Core/ResourceManager.hpp"
#include "Scene/Level.hpp"
#include "Utility/logger.hpp"
#include "Core/JSONLoader.hpp"

#include "Base/Constants.hpp"

void Weather::update() {
    if(overlayActive && overlayOpacityChange != 0.0f) {
        overlayOpacity += overlayOpacityChange;
        overlayOpacity = std::max(overlayOpacity, 0.0f);
        overlayOpacity = std::min(overlayOpacity, 1.0f);
    }
}

Weather::Weather(WeatherType weatherType, const std::string& id, Level& level) 
    : type(weatherType), id(id), level(level), overlayActive(false), overlaySprite(GameConstants::BLANK_TEXTURE) {
        stat = level.getDifficulty().getWeatherModifier(id);
        overlayOpacity = 1.0f;
        overlayOpacityChange = 0.0f;
}

void Weather::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (overlayActive) {
        target.draw(overlaySprite, states);
    }
}

void Weather::loadOverlay(const std::string& weatherId) {
    const nlohmann::json& weatherData = JSONLoader::getInstance().getWeather(weatherId);

    overlayActive = false;
    if (weatherData.contains("has_overlay")) {
        overlayActive = weatherData["has_overlay"];
    }
    
    if (overlayActive) {
        if (weatherData.contains("overlay_animation")) {
            try {
                overlayAnimation.loadJson(weatherData["overlay_animation"]);
                Logger::debug("Weather: Loaded overlay animation for " + weatherId);
            } catch (const std::exception& e) {
                overlayActive = false;
                Logger::error("Weather: Failed to load overlay animation for " + weatherId + ": " + e.what());
            }
        } else {
            overlayActive = false;
            Logger::error("Weather: No overlay animation defined for " + weatherId);
        }
    }
}

void Weather::applyToTower(Tower* tower) {
    EntityStat* stats = tower->getStats();
    stats->addStat(EntityStat::multiplier("range"), -stat.getTowerRangeReduction());
}

void Weather::removeFromTower(Tower* tower) {
    EntityStat* stats = tower->getStats();
    stats->addStat(EntityStat::multiplier("range"), stat.getTowerRangeReduction());
}

void Weather::applyToEnemy(Enemy* enemy) {
    // EntityStat* stats = enemy->getStats();
    // stats->addStat(EntityStat::multiplier("speed"), -stat.getEnemySpeedReduction());
}

void Weather::removeFromEnemy(Enemy* enemy) {
    // EntityStat* stats = enemy->getStats();
    // stats->addStat(EntityStat::multiplier("speed"), stat.getEnemySpeedReduction());
}