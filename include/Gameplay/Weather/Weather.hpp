/**
 * @file Weather.hpp
 * @brief Declares weather types and base Weather class.
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Entity/Modules/SpriteAnimation.hpp"

/**
 * @enum WeatherType
 * @brief Different types of weather conditions.
 */
enum class WeatherType {
    Sunny,
    Raining,
    Thunderstorm,
    Foggy
};

// Forward declarations
class Tower;
class Enemy;
class Level;

/**
 * @class Weather
 * @brief Base class for weather effects that modify gameplay.
 */
class Weather : public sf::Drawable {
protected:
    WeatherType type;
    Level& level;

    bool overlayActive;
    float overlayOpacity;
    float overlayOpacityChange;
    sf::Sprite overlaySprite;
    SpriteAnimation overlayAnimation;

public:
    /**
     * @brief Construct a new Weather object.
     * @param weatherType The type of weather.
     * @param level Reference to the game level.
     */
    Weather(WeatherType weatherType, Level& level);
    
    /**
     * @brief Virtual destructor.
     */
    virtual ~Weather() = default;

    /**
     * @brief Update weather effects.
     */
    virtual void update() = 0;

    /**
     * @brief Apply weather effects to a tower.
     * @param tower Reference to the tower.
     */
    virtual void applyToTower(Tower* tower) = 0;

    /**
     * @brief Apply weather effects to an enemy.
     * @param enemy Reference to the enemy.
     */
    virtual void applyToEnemy(Enemy* enemy) = 0;

    /**
     * @brief Remove weather effects from a tower.
     * @param tower Reference to the tower.
     */
    virtual void removeFromTower(Tower* tower) = 0;

    /**
     * @brief Remove weather effects from an enemy.
     * @param enemy Reference to the enemy.
     */
    virtual void removeFromEnemy(Enemy* enemy) = 0;

    /**
     * @brief Draw the weather overlay.
     * @param target Render target.
     * @param states Render states.
     */
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    /**
     * @brief Get the weather type.
     * @return WeatherType The type of this weather.
     */
    WeatherType getType() const { return type; }

    /**
     * @brief Check if overlay is active.
     * @return bool True if overlay should be drawn.
     */
    bool isOverlayActive() const { return overlayActive; }

    inline float getOverlayOpacity() const { return overlayOpacity; }
    inline float getOverlayOpacityChange() const { return overlayOpacityChange; }
    inline void setOverlayOpacity(float opacity) { overlayOpacity = opacity; }
    inline void setOverlayOpacityChange(float change) { overlayOpacityChange = change; }

protected:
    /**
     * @brief Load overlay texture for this weather.
     */
    void loadOverlay(const std::string& weatherId);
};