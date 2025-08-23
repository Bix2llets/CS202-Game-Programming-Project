#pragma once

#include <Entity/Tower/Behaviors/Combat/FireMode.hpp>
#include <Entity/Tower/Behaviors/Combat/TargetSelector.hpp>
#include <Gameplay/Currency.hpp>
#include <stdexcept>
#include <vector>

// Forward declarations to avoid circular dependencies
class Tower;
class Enemy;

enum class BehaviorType { Combat, Resource, Glowing };

class TowerBehavior {
    protected:
    Tower* base;

    public:
    explicit TowerBehavior(Tower* tower) : base(tower) {}
    virtual ~TowerBehavior() = default;
    virtual BehaviorType getType() const = 0;

    /**
     * @brief Get the tower that owns this behavior.
     * @return Pointer to the owning tower, or nullptr if not assigned
     */
    Tower* getTower() const { return base; }

    /**
     * @brief Set the tower reference for this behavior.
     * This method only works if the current tower reference is null,
     * ensuring that behaviors can only be linked to exactly one tower.
     *
     * @param tower Pointer to the tower that will own this behavior
     * @throws std::runtime_error If trying to set tower when behavior already
     * has a base
     * @throws std::invalid_argument If tower pointer is null
     */
    void setTower(Tower* tower) {
        if (tower == nullptr) {
            throw std::invalid_argument(
                "Cannot set null tower pointer to behavior");
        }
        if (base != nullptr) {
            throw std::runtime_error(
                "Behavior already has a tower base - cannot reassign to "
                "another tower");
        }
        base = tower;
    }
};

using namespace Combat;

class CombatBehavior : public TowerBehavior {
    private:
    TargetSelector* targetSelector;  ///< Selector for targets to attack
    FireMode* fireMode;              ///< Mode of firing at targets
    public:
    CombatBehavior(Tower* tower, TargetSelector* selector, FireMode* mode)
        : TowerBehavior(tower), targetSelector(selector), fireMode(mode) {}

    ~CombatBehavior() override {
        delete targetSelector;
        delete fireMode;
    }

    BehaviorType getType() const override { return BehaviorType::Combat; }

    /**
     * @brief Select targets and fire at them
     *
     * @param enemies List of available enemies to target
     */
    int engage(const std::vector<Enemy*>& enemies) const;

    inline const TargetSelector* getTargetSelector() const {
        return targetSelector;
    };

    inline void setTargetSelector(TargetSelector* selector) {
        if (selector == nullptr) {
            throw std::invalid_argument(
                "Cannot set null target selector to combat behavior");
        }
        delete targetSelector;
        targetSelector = selector;
    };
};

class ResourceBehavior : public TowerBehavior {
    private:
    Currency resourceAmount;  ///< Amount of resources to generate
    public:
    explicit ResourceBehavior(Tower* tower, const Currency& amount)
        : TowerBehavior(tower), resourceAmount(amount) {}

    ~ResourceBehavior() override = default;

    BehaviorType getType() const override { return BehaviorType::Resource; }

    void generate(Currency& currentResource) const;
};

class GlowingBehavior : public TowerBehavior {
    public:
    explicit GlowingBehavior(Tower* tower) : TowerBehavior(tower) {}

    ~GlowingBehavior() override = default;

    BehaviorType getType() const override { return BehaviorType::Glowing; }
};