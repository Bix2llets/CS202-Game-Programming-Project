#pragma once

#include <vector>

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
    virtual void fire(const Tower* tower, std::vector<Enemy*> target) const = 0;
};

class InstantFireMode : public FireMode {
public:
    void fire(const Tower* tower, std::vector<Enemy*> target) const override;
};

class ProjectileFireMode : public FireMode {
public:
    void fire(const Tower* tower, std::vector<Enemy*> target) const override;
};

class ContinuousFireMode : public FireMode {
public:
    void fire(const Tower* tower, std::vector<Enemy*> target) const override;
};

}