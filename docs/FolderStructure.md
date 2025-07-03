# Entity System Folder Structure

This document describes the reorganized folder structure for the Entity System.

## New Organization

The Entity System has been reorganized into logical subfolders:

### Root Entity (`include/Entity/`, `src/Entity/`)
- `Entity.hpp` / `Entity.cpp` - Base entity class
- `Damageable.hpp` - Interface for damageable entities

### Tower System (`include/Entity/Tower/`, `src/Entity/Tower/`)
- `Tower.hpp` / `Tower.cpp` - Tower entity implementation
- `Projectile.hpp` / `Projectile.cpp` - Projectile entity (Tower-related)
- `AttackStrategy.hpp` - Attack strategy interface
- `SingleTargetAttack.hpp` / `SingleTargetAttack.cpp` - Single target attack implementation
- `AreaOfEffectAttack.hpp` / `AreaOfEffectAttack.cpp` - Area of effect attack implementation

### Enemy System (`include/Entity/Enemy/`, `src/Entity/Enemy/`)
- `Enemy.hpp` / `Enemy.cpp` - Enemy entity implementation
- `EnemyState.hpp` - Enemy state interface
- `MovingState.hpp` / `MovingState.cpp` - Moving state implementation
- `AttackingState.hpp` / `AttackingState.cpp` - Attacking state implementation
- `DyingState.hpp` / `DyingState.cpp` - Dying state implementation

## Updated Include Paths

All include statements have been updated to reflect the new structure:

```cpp
// Main header (includes everything)
#include "EntitySystem.hpp"

// Direct includes (if needed)
#include "Entity/Entity.hpp"
#include "Entity/Damageable.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/Tower/Projectile.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Enemy/EnemyState.hpp"
// ... etc
```

## Benefits

1. **Better Organization**: Related classes are grouped together
2. **Clearer Dependencies**: Tower-related classes are separate from Enemy-related classes
3. **Easier Maintenance**: Changes to one subsystem don't affect others
4. **Scalability**: Easy to add new features to specific subsystems

## Build System

The CMake build system automatically detects all source files in the new structure using recursive file globbing. No manual file listing is required.
