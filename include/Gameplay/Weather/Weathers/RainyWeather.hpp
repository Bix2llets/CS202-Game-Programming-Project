/**
 * @file RainyWeather.hpp
 * @brief Declares the RainyWeather class.
 */
#pragma once

#include "Gameplay/Weather/Weather.hpp"

/**
 * @class RainyWeather
 * @brief Rainy weather that reduces tower range and enemy speed, affects burn effects.
 */
class RainyWeather : public Weather {
private:
    static constexpr float TOWER_RANGE_REDUCTION = 0.15f; // 15% reduction
    static constexpr float ENEMY_SPEED_REDUCTION = 0.10f; // 10% reduction
    static constexpr float BURN_DAMAGE_REDUCTION = 0.40f; // 40% reduction
    static constexpr float BURN_DURATION_REDUCTION = 0.40f; // 40% reduction

public:
    RainyWeather(Level& level);
    
    void update() override;
    void applyToTower(Tower* tower) override;
    void applyToEnemy(Enemy* enemy) override;
    void removeFromTower(Tower* tower) override;
    void removeFromEnemy(Enemy* enemy) override;
};
