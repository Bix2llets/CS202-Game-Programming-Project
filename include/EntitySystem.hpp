
#pragma once

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

// Target Selection Strategy pattern

// Enemy State pattern
#include "Entity/Enemy/EnemyState.hpp"
#include "Entity/Enemy/MovingState.hpp"
#include "Entity/Enemy/AttackingState.hpp"
#include "Entity/Enemy/DyingState.hpp"


