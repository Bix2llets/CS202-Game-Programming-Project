/**
 * @file RainingWeather.hpp
 * @brief Declares the RainingWeather class.
 */
#pragma once

#include "Gameplay/Weather/Weather.hpp"

/**
 * @class ThunderstormWeather
 * @brief Thunderstorm weather that reduces tower range and enemy speed, affects burn effects.
 */
class ThunderstormWeather : public Weather {
public:
    ThunderstormWeather(Level& level);

    void update() override;
    void applyToTower(Tower* tower) override;
    void applyToEnemy(Enemy* enemy) override;
    void removeFromTower(Tower* tower) override;
    void removeFromEnemy(Enemy* enemy) override;

};
