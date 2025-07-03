#ifndef ENTITY_SYSTEM_HPP
#define ENTITY_SYSTEM_HPP

/**
 * @file EntitySystem.hpp
 * @brief Main header file for the Entity System
 * 
 * Include this file to access all Entity System components.
 */

// Base Entity class
#include "Entity/Entity.hpp"

// Damageable interface
#include "Entity/Damageable.hpp"

// Base Entity types
#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/Tower/Projectile.hpp"

// Tower types
#include "Entity/Tower/Combat/CombatTower.hpp"
#include "Entity/Tower/Combat/InstantTower.hpp"
#include "Entity/Tower/Combat/ProjectileTower.hpp"
#include "Entity/Tower/Combat/ContinuousTower.hpp"
#include "Entity/Tower/Passive/PassiveTower.hpp"
#include "Entity/Tower/Resource/ResourceTower.hpp"

// Target Selection Strategy pattern
#include "Entity/Tower/Combat/TargetSelection.hpp"

// Enemy State pattern
#include "Entity/Enemy/EnemyState.hpp"
#include "Entity/Enemy/MovingState.hpp"
#include "Entity/Enemy/AttackingState.hpp"
#include "Entity/Enemy/DyingState.hpp"

#endif // ENTITY_SYSTEM_HPP
