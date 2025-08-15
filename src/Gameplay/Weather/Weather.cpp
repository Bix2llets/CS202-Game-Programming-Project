/**
 * @file Weather.cpp
 * @brief Implementation of the base Weather class.
 */

#include "Gameplay/Weather/Weather.hpp"
#include "Core/ResourceManager.hpp"
#include "Scene/Level.hpp"
#include "Utility/logger.hpp"

#include "Base/Constants.hpp"

Weather::Weather(WeatherType weatherType, Level& level) 
    : type(weatherType), level(level), overlayActive(false), overlay(GameConstants::BLANK_TEXTURE) {
}

void Weather::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (overlayActive) {
        target.draw(overlay, states);
    }
}

void Weather::loadOverlay(const std::string& textureId) {
    try {
        const sf::Texture* texture = ResourceManager::getInstance().getTexture(textureId);
        if (texture) {
            overlay.setTexture(*texture);
            overlayActive = true;
            Logger::debug("Weather: Successfully loaded overlay texture: " + textureId);
        } else {
            Logger::warning("Weather: Failed to load overlay texture: " + textureId);
            overlayActive = false;
        }
    } catch (const std::exception& e) {
        Logger::error("Weather: Exception loading overlay texture " + textureId + ": " + e.what());
        overlayActive = false;
    }
}
