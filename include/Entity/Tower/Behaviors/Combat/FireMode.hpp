#pragma once

#include <vector>
#include <memory>
#include "Entity/Tower/Projectile/Projectile.hpp"

class Tower;
class Enemy;

namespace Combat {

/**
 * @brief Abstract base class for tower fire modes
 */
class FireMode {
public:
    virtual ~FireMode() = default;

    /**
     * @brief Fire at the selected target
     * 
     * @param tower The combat tower firing
     * @param target The target to fire at
     */
    virtual int fire(Tower* tower, std::vector<Enemy*>& target) const = 0;
};

class InstantFireMode : public FireMode {
public:
    int fire(Tower* tower, std::vector<Enemy*>& target) const override;
};

class ProjectileFireMode : public FireMode {
private:
    std::unique_ptr<Projectile> projectile; ///< Projectile to be fired
    friend class TowerFactory; ///< Allow TowerFactory to set projectile
public:
    void setProjectile(std::unique_ptr<Projectile> proj);
    int fire(Tower* tower, std::vector<Enemy*>& target) const override;
};

class ContinuousFireMode : public FireMode {
public:
    int fire(Tower* tower, std::vector<Enemy*>& target) const override;
};

}