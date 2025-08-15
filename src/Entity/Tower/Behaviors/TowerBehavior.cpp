#include "Entity/Tower/Behaviors/TowerBehavior.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/Modules/EntityStat.hpp"
#include "Entity/Enemy/Enemy.hpp"

int CombatBehavior::engage(const std::vector<Enemy*>& enemies) const {
    auto targets = targetSelector->selectTarget(base, enemies, base->getStat(TowerStat::MAX_TARGETS));
    if (!targets.empty()) {
        base->setMainTarget(targets[0]); // Set the first target as the main target
        fireMode->fire(base, targets);
        
        if(base->getMainTarget() != nullptr) {
            base->setMainTarget(nullptr); // Clear main target if it's not alive
        }
        return true;
    }

    return targets.size(); // Return number of targets engaged
}

void ResourceBehavior::generate(Currency& currentResource) const {
    currentResource += resourceAmount; // Add generated resources to the current total
}
