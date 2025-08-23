#pragma once

#include "Entity/Tower/Behaviors/Combat/TargetSelector.hpp"
namespace SelectorCycle {
    extern Combat::TargetSelector* nextSelector(const Combat::TargetSelector* current);
    extern Combat::TargetSelector* prevSelector(const Combat::TargetSelector* current);
};