#pragma once

#include <Entity/Tower/Behaviors/Combat/TargetSelector.hpp>
#include <Entity/Tower/Behaviors/Combat/FireMode.hpp>
#include <Gameplay/Currency.hpp>
#include <vector>

// Forward declarations to avoid circular dependencies
class Tower;
class Enemy;

enum class BehaviorType {
    Combat,
    Resource,
    Glowing
};

class TowerBehavior {
protected:
    const Tower* base;
public:
    explicit TowerBehavior(const Tower* tower) : base(tower) {}
    virtual ~TowerBehavior() = default;
    virtual BehaviorType getType() const = 0;
};

using namespace Combat;

class CombatBehavior : public TowerBehavior {
private:
    TargetSelector* targetSelector; ///< Selector for targets to attack
    FireMode* fireMode; ///< Mode of firing at targets  
public:
    CombatBehavior(const Tower* tower, TargetSelector* selector, FireMode* mode)
        : TowerBehavior(tower), targetSelector(selector), fireMode(mode) {}

    ~CombatBehavior() override {
        delete targetSelector;
        delete fireMode;
    }

    BehaviorType getType() const override {
        return BehaviorType::Combat;
    }

    /**
     * @brief Select targets and fire at them
     * 
     * @param enemies List of available enemies to target
     */
    void engage(const std::vector<Enemy*>& enemies) const;
};

class ResourceBehavior : public TowerBehavior {
private:
    Currency resourceAmount; ///< Amount of resources to generate
public:
    explicit ResourceBehavior(const Tower* tower, const Currency& amount)
        : TowerBehavior(tower), resourceAmount(amount) {}

    ~ResourceBehavior() override = default;

    BehaviorType getType() const override {
        return BehaviorType::Resource;
    }

    void generate(Currency& currentResource) const;
};

class GlowingBehavior : public TowerBehavior {
public:
    explicit GlowingBehavior(const Tower* tower) : TowerBehavior(tower) {}
    
    ~GlowingBehavior() override = default;
    
    BehaviorType getType() const override {
        return BehaviorType::Glowing;
    }
};