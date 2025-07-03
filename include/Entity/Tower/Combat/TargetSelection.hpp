
#pragma once

#include <vector>

class Enemy;
class CombatTower;

namespace Combat {

/**
 * @brief Abstract base class for tower target selection strategies
 */
class TargetSelection {
   public:
    virtual ~TargetSelection() = default;

    /**
     * @brief Select a target from available enemies
     *
     * @param tower The combat tower selecting the target
     * @param enemies List of available enemy targets
     * @return Enemy* Selected enemy target or nullptr if no valid target found
     */
    virtual Enemy* selectTarget(const CombatTower* tower,
                                const std::vector<Enemy*>& enemies) const = 0;
};

/**
 * @brief Strategy to select the nearest enemy to the tower
 */
class NearestTargetSelection : public TargetSelection {
   public:
    /**
     * @brief Select the nearest enemy to the tower
     *
     * @param tower The combat tower selecting the target
     * @param enemies List of available enemy targets
     * @return Enemy* The nearest enemy or nullptr if no valid target found
     */
    Enemy* selectTarget(const CombatTower* tower,
                        const std::vector<Enemy*>& enemies) const override;
};

/**
 * @brief Strategy to select the farthest enemy from the tower
 */
class FarthestTargetSelection : public TargetSelection {
   public:
    /**
     * @brief Select the farthest enemy from the tower
     *
     * @param tower The combat tower selecting the target
     * @param enemies List of available enemy targets
     * @return Enemy* The farthest enemy or nullptr if no valid target found
     */
    Enemy* selectTarget(const CombatTower* tower,
                        const std::vector<Enemy*>& enemies) const override;
};

/**
 * @brief Strategy to select the enemy with the lowest health
 */
class LowestHealthTargetSelection : public TargetSelection {
   public:
    /**
     * @brief Select the enemy with the lowest health
     *
     * @param tower The combat tower selecting the target
     * @param enemies List of available enemy targets
     * @return Enemy* The enemy with lowest health or nullptr if no valid target
     * found
     */
    Enemy* selectTarget(const CombatTower* tower,
                        const std::vector<Enemy*>& enemies) const override;
};

/**
 * @brief Strategy to select the enemy with the highest health
 */
class HighestHealthTargetSelection : public TargetSelection {
   public:
    /**
     * @brief Select the enemy with the highest health
     *
     * @param tower The combat tower selecting the target
     * @param enemies List of available enemy targets
     * @return Enemy* The enemy with highest health or nullptr if no valid
     * target found
     */
    Enemy* selectTarget(const CombatTower* tower,
                        const std::vector<Enemy*>& enemies) const override;
};

}  // namespace Combat
