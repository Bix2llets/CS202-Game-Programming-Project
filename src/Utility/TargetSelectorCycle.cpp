#include "Utility/TargetSelectorCycle.hpp"

Combat::TargetSelector* SelectorCycle::nextSelector(const Combat::TargetSelector* current) {
    if (dynamic_cast<const Combat::FirstTargetSelector*>(current))
        return new Combat::LastTargetSelector();
    if (dynamic_cast<const Combat::LastTargetSelector*>(current))
        return new Combat::HighestHealthTargetSelector();
    if (dynamic_cast<const Combat::HighestHealthTargetSelector*>(current))
        return new Combat::LowestHealthTargetSelector();
    if (dynamic_cast<const Combat::LowestHealthTargetSelector*>(current))
        return new Combat::NearestTargetSelector();
    if (dynamic_cast<const Combat::NearestTargetSelector*>(current))
        return new Combat::FarthestTargetSelector();
    if (dynamic_cast<const Combat::FarthestTargetSelector*>(current))
        return new Combat::FirstTargetSelector();
    return new Combat::FirstTargetSelector();
}
Combat::TargetSelector* SelectorCycle::prevSelector(const Combat::TargetSelector* current) {
    if (dynamic_cast<const Combat::FirstTargetSelector*>(current))
        return new Combat::FarthestTargetSelector();
    if (dynamic_cast<const Combat::FarthestTargetSelector*>(current))
        return new Combat::NearestTargetSelector();
    if (dynamic_cast<const Combat::NearestTargetSelector*>(current))
        return new Combat::LowestHealthTargetSelector();
    if (dynamic_cast<const Combat::LowestHealthTargetSelector*>(current))
        return new Combat::HighestHealthTargetSelector();
    if (dynamic_cast<const Combat::HighestHealthTargetSelector*>(current))
        return new Combat::LastTargetSelector();
    if (dynamic_cast<const Combat::LastTargetSelector*>(current))
        return new Combat::FirstTargetSelector();
    return new Combat::FirstTargetSelector();
}