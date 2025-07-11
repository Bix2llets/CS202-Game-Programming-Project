#pragma once

#include <vector>

class Tower;
class Enemy;

namespace Combat {

/**
 * @brief Abstract base class for tower target selection strategies
 */
class TargetSelector {
public:
   virtual ~TargetSelector() = default;

   /**
    * @brief Select targets from available enemies
    *
    * @param tower The combat tower selecting the targets
    * @param enemies List of available enemy targets
    * @param maxTargets Maximum number of targets to select (default = 1)
    * @return std::vector<Enemy*> Selected enemy targets (up to maxTargets)
    */
   virtual std::vector<Enemy*> selectTarget(const Tower* tower, const std::vector<Enemy*>& enemies, int maxTargets = 1) const = 0;
};

/**
 * @brief Strategy to select the nearest enemy to the tower
 */
class NearestTargetSelector : public TargetSelector {
public:
   /**
    * @brief Select the nearest enemies to the tower
    *
    * @param tower The combat tower selecting the targets
    * @param enemies List of available enemy targets
    * @param maxTargets Maximum number of targets to select (default = 1)
    * @return std::vector<Enemy*> The nearest enemies (up to maxTargets)
    */
   std::vector<Enemy*> selectTarget(const Tower* tower, const std::vector<Enemy*>& enemies, int maxTargets = 1) const override;
};

/**
 * @brief Strategy to select the farthest enemy from the tower
 */
class FarthestTargetSelector : public TargetSelector {
public:
   /**
    * @brief Select the farthest enemies from the tower
    *
    * @param tower The combat tower selecting the targets
    * @param enemies List of available enemy targets
    * @param maxTargets Maximum number of targets to select (default = 1)
    * @return std::vector<Enemy*> The farthest enemies (up to maxTargets)
    */
   std::vector<Enemy*> selectTarget(const Tower* tower, const std::vector<Enemy*>& enemies, int maxTargets = 1) const override;
};

/**
 * @brief Strategy to select the enemy with the lowest health
 */
class LowestHealthTargetSelector : public TargetSelector {
public:
   /**
    * @brief Select the enemies with the lowest health
    *
    * @param tower The combat tower selecting the targets
    * @param enemies List of available enemy targets
    * @param maxTargets Maximum number of targets to select (default = 1)
    * @return std::vector<Enemy*> The enemies with lowest health (up to maxTargets)
    */
   std::vector<Enemy*> selectTarget(const Tower* tower, const std::vector<Enemy*>& enemies, int maxTargets = 1) const override;
};

/**
 * @brief Strategy to select the enemy with the highest health
 */
class HighestHealthTargetSelector : public TargetSelector {
public:
   /**
    * @brief Select the enemies with the highest health
    *
    * @param tower The combat tower selecting the targets
    * @param enemies List of available enemy targets
    * @param maxTargets Maximum number of targets to select (default = 1)
    * @return std::vector<Enemy*> The enemies with highest health (up to maxTargets)
    */
   std::vector<Enemy*> selectTarget(const Tower* tower, const std::vector<Enemy*>& enemies, int maxTargets = 1) const override;
};

}  // namespace Combat
