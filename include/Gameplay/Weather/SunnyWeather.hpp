/**
 * @file SunnyWeather.hpp
 * @brief Declares the SunnyWeather class.
 */
#pragma once

#include "Weather.hpp"

/**
 * @class SunnyWeather
 * @brief Default sunny weather with no special effects.
 */
class SunnyWeather : public Weather {
public:
    SunnyWeather(Level& level);
    
    void update() override;
    void applyToTower(Tower& tower) override;
    void applyToEnemy(Enemy& enemy) override;
    void removeFromTower(Tower& tower) override;
    void removeFromEnemy(Enemy& enemy) override;
};
