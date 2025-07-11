#include "Entity/Tower/Behaviors/TowerBehavior.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/Tower/TowerStat.hpp"

void CombatBehavior::engage(const std::vector<Enemy*>& enemies) const {
    auto targets = targetSelector->selectTarget(base, enemies, base->getStat(TowerStat::MAX_TARGETS));
    if (!targets.empty()) {
        fireMode->fire(base, targets);
    }
}

void ResourceBehavior::generate(Currency& currentResource) const {
    currentResource += resourceAmount; // Add generated resources to the current total
}
