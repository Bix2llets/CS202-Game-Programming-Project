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

Weather::Weather(WeatherType weatherType, Level& level) 
    : type(weatherType), level(level), overlayActive(false), overlaySprite(GameConstants::BLANK_TEXTURE) {
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
