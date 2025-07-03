# Entity System Documentation

## Overview

The Entity System is a comprehensive game object framework implemented using several design patterns to provide flexible and extensible game entity management. The system is based on the UML diagram provided and implements the following key components:

## Architecture

### Base Entity Class
- **Entity**: Abstract base class providing common functionality for all game objects
  - Position and rotation management
  - Sprite rendering (optional)
  - Unique entity ID system
  - Alive/dead state management

### Damageable Interface
- **Damageable**: Interface for entities that can take damage
  - Health and maximum health management
  - Damage and healing system
  - Death handling callbacks
  - Health percentage calculations

### Concrete Entity Types

#### 1. Enemy (Entity + Damageable)
- Represents hostile entities that move along predefined paths
- Implements State pattern for behavior management
- Supports different enemy types (Basic, Fast, Heavy, Flying)
- Provides reward system for player progression
- Can take damage and be destroyed

#### 2. Tower (Entity + Damageable)
- Defensive structures that attack enemies within range
- Implements Strategy pattern for different attack types
- Supports upgrades and level progression
- Configurable range, damage, and attack speed
- Can be damaged and destroyed

#### 3. Projectile (Entity only)
- Fired by towers to attack enemies
- Handles movement and collision detection
- Supports different projectile types and behaviors
- No health system (destroyed on impact)

#### 4. PlayerUnit (Entity + Damageable)
- Player-controlled entities
- Command system for player input
- Selection and control mechanisms
- Different unit types with varying capabilities
- Can take damage and be destroyed

## Design Patterns Implemented

### 1. Strategy Pattern (Attack Strategies)
The tower attack system uses the Strategy pattern to allow different attack behaviors:

- **AttackStrategy**: Abstract interface
- **SingleTargetAttack**: Attacks one enemy at a time
- **AreaOfEffectAttack**: Attacks multiple enemies in a radius

```cpp
// Example: Changing tower attack strategy
tower->setAttackStrategy(std::make_unique<AreaOfEffectAttack>(75.0f));
```

### 2. State Pattern (Enemy Behavior)
Enemy behavior is managed using the State pattern:

- **EnemyState**: Abstract base state
- **MovingState**: Enemy moving along path
- **AttackingState**: Enemy engaging in combat
- **DyingState**: Enemy death sequence

```cpp
// State transitions happen automatically based on conditions
enemy->changeState(std::make_unique<AttackingState>());
```

### 3. Interface Segregation Pattern
The Damageable interface separates health-related functionality from the base Entity class:

- **Damageable**: Interface for entities that can take damage
- **Implemented by**: Enemy, Tower, PlayerUnit
- **Not implemented by**: Projectile (no health needed)

```cpp
// Only entities that need health implement Damageable
class Enemy : public Entity, public Damageable {
    // Has health, can take damage
};

class Projectile : public Entity {
    // No health system, destroyed on impact
};
```

### 4. Template Method Pattern
The base Entity class provides template methods that derived classes can override while maintaining common structure.

## Key Features

### Entity Management
- Unique ID system for entity tracking
- Automatic sprite positioning and rotation synchronization
- Optional sprite system (entities don't require sprites)
- Alive/dead state tracking
- Clean separation of concerns (health only where needed)

### Health System (Damageable Interface)
- Health and maximum health management
- Damage dealing with automatic death handling
- Healing system with maximum health limits
- Health percentage calculations
- Polymorphic damage handling

### Tower System
- Range-based targeting
- Multiple attack strategies
- Upgrade system with stat improvements
- Attack cooldown management

### Enemy System
- Pathfinding and waypoint navigation
- State-based behavior management
- Multiple enemy types with different characteristics
- Reward system integration

### Projectile System
- Target tracking and collision detection
- Configurable speed and damage
- Automatic cleanup on impact

### Player Unit System
- Selection and command management
- Multiple unit types
- Visual selection indicators

## Usage Examples

### Creating Entities
```cpp
// Create a projectile (no health)
auto projectile = std::make_unique<Projectile>(
    sf::Vector2f(100, 100),  // Position
    nullptr,                 // Target
    25,                      // Damage
    200.0f                   // Speed
);

// Create damageable entities
auto enemy = std::make_unique<Enemy>(
    sf::Vector2f(200, 200),  // Position
    80,                      // Health
    50.0f,                   // Speed
    EnemyType::Basic         // Type
);

auto tower = std::make_unique<Tower>(
    sf::Vector2f(100, 100),  // Position
    200,                     // Health
    150.0f,                  // Range
    30,                      // Damage
    TowerType::Basic         // Type
);
```

### Using Damageable Interface
```cpp
// Polymorphic damage handling
std::vector<Damageable*> damageableEntities;
damageableEntities.push_back(enemy.get());
damageableEntities.push_back(tower.get());

// Damage all entities uniformly
for (auto* entity : damageableEntities) {
    entity->takeDamage(25);
    std::cout << "Health: " << entity->getHealthPercentage() * 100 << "%" << std::endl;
}
```

### Using Strategy Pattern
```cpp
// Change tower to use area of effect attacks
tower->setAttackStrategy(std::make_unique<AreaOfEffectAttack>(75.0f));

// Attack enemies (strategy determines behavior)
std::vector<Enemy*> enemies = {enemy1, enemy2, enemy3};
tower->attack(enemies);
```

### Managing Entity States
```cpp
// Enemy states change automatically based on conditions
enemy->update(deltaTime);  // Updates current state

// Manual state changes are also possible
enemy->changeState(std::make_unique<AttackingState>());
```

### Player Unit Control
```cpp
// Select unit
playerUnit->select(true);

// Give command
playerUnit->addCommand("Move to position (150, 150)");

// Check selection status
if (playerUnit->isSelected()) {
    // Handle selected unit input
}
```

## Integration with Game Loop

```cpp
void gameUpdate(float deltaTime) {
    // Update all entities
    for (auto& entity : entities) {
        entity->update(deltaTime);
    }
    
    // Handle tower attacks
    for (auto& tower : towers) {
        tower->attack(enemies);
    }
    
    // Update projectiles
    for (auto& projectile : projectiles) {
        projectile->update(deltaTime);
        
        // Remove destroyed projectiles
        if (!projectile->isAlive()) {
            // Remove from container
        }
    }
}

void gameRender(sf::RenderTarget& target) {
    // Render all entities
    for (const auto& entity : entities) {
        entity->render(target);
    }
}
```

## File Structure

```
include/Entity/
├── Entity.hpp              # Base entity class
├── Damageable.hpp          # Health interface
├── Enemy.hpp               # Enemy entity
├── Tower.hpp               # Tower entity
├── Projectile.hpp          # Projectile entity
├── PlayerUnit.hpp          # Player unit entity
├── AttackStrategy.hpp      # Strategy pattern interface
├── SingleTargetAttack.hpp  # Single target strategy
├── AreaOfEffectAttack.hpp  # AoE attack strategy
├── EnemyState.hpp          # State pattern interface
├── MovingState.hpp         # Moving state
├── AttackingState.hpp      # Attacking state
└── DyingState.hpp          # Dying state

src/Entity/
├── Entity.cpp              # Base entity implementation
├── Enemy.cpp               # Enemy implementation
├── Tower.cpp               # Tower implementation
├── Projectile.cpp          # Projectile implementation
├── PlayerUnit.cpp          # Player unit implementation
├── SingleTargetAttack.cpp  # Single target strategy impl
├── AreaOfEffectAttack.cpp  # AoE attack strategy impl
├── MovingState.cpp         # Moving state impl
├── AttackingState.cpp      # Attacking state impl
└── DyingState.cpp          # Dying state impl

include/
└── EntitySystem.hpp        # Main include file for entire system
```

## SFML 3.0 Compatibility

The Entity System is designed for SFML 3.0 and includes:
- Proper use of `sf::Angle` for rotation (instead of float)
- Updated drawing and rendering methods
- Modern C++ practices (C++20)

## Future Enhancements

Potential areas for expansion:
1. **Component System**: Convert to Entity-Component-System (ECS) architecture
2. **Animation System**: Add sprite animation support
3. **Physics Integration**: Add collision detection and physics
4. **Serialization**: Save/load entity states
5. **Performance Optimization**: Spatial partitioning for entity queries
6. **AI Enhancements**: More sophisticated enemy AI states
7. **Particle Systems**: Visual effects for attacks and explosions

## Example Program

See `examples/EntitySystemDemo.cpp` for a complete working example demonstrating all system features.
