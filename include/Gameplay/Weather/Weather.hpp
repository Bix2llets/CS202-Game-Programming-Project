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
enum class WeatherType { Sunny, Raining, Thunderstorm, Foggy };

// Forward declarations
class Tower;
class Enemy;
class Level;

class WeatherStat {
    private:
    float towerRangeReduction;
    float enemySpeedReduction;
    float burnDamageReduction;
    float burnDurationReduction;

    public:
    WeatherStat() {
        towerRangeReduction = 0.0f;
        enemySpeedReduction = 0.0f;
        burnDamageReduction = 0.0f;
        burnDurationReduction = 0.0f;
    }

    float getTowerRangeReduction() const { return towerRangeReduction; }
    void setTowerRangeReduction(float value) { towerRangeReduction = value; }

    float getEnemySpeedReduction() const { return enemySpeedReduction; }
    void setEnemySpeedReduction(float value) { enemySpeedReduction = value; }

    float getBurnDamageReduction() const { return burnDamageReduction; }
    void setBurnDamageReduction(float value) { burnDamageReduction = value; }

    float getBurnDurationReduction() const { return burnDurationReduction; }
    void setBurnDurationReduction(float value) {
        burnDurationReduction = value;
    }

    void loadFromJson(const nlohmann::json& statsJson) {
        if (statsJson.contains("tower_range_reduction")) {
            towerRangeReduction =
                statsJson["tower_range_reduction"].get<float>();
        }
        if (statsJson.contains("enemy_speed_reduction")) {
            enemySpeedReduction =
                statsJson["enemy_speed_reduction"].get<float>();
        }
        if (statsJson.contains("burn_damage_reduction")) {
            burnDamageReduction =
                statsJson["burn_damage_reduction"].get<float>();
        }
        if (statsJson.contains("burn_duration_reduction")) {
            burnDurationReduction =
                statsJson["burn_duration_reduction"].get<float>();
        }
    }
};

/**
 * @class Weather
 * @brief Base class for weather effects that modify gameplay.
 */
class Weather : public sf::Drawable {
    protected:
    std::string id;
    WeatherType type;
    WeatherStat stat;
    Level& level;

    bool overlayActive;
    float overlayOpacity;
    float overlayOpacityChange;
    sf::Sprite overlaySprite;
    sf::Sprite icon;
    SpriteAnimation overlayAnimation;

    public:
    /**
     * @brief Construct a new Weather object.
     * @param weatherType The type of weather.
     * @param id The unique identifier for this weather.
     * @param level Reference to the game level.
     */
    Weather(WeatherType weatherType, const std::string& id, Level& level);

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
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const override;

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
    inline float getOverlayOpacityChange() const {
        return overlayOpacityChange;
    }
    inline void setOverlayOpacity(float opacity) { overlayOpacity = opacity; }
    inline void setOverlayOpacityChange(float change) {
        overlayOpacityChange = change;
    }

    inline sf::Sprite getIcon() const { return icon; };

    protected:
    /**
     * @brief Load overlay texture for this weather.
     */
    void loadTexture(const std::string& weatherId);
};