# TowerFactory Documentation

## Overview

The `TowerFactory` class provides a complete JSON-driven tower creation system with full support for:
- Basic tower configuration (stats, textures, cost)
- Advanced upgrade system with multiple upgrade types
- Behavior system integration (placeholder for future implementation)
- Flexible builder pattern integration

## JSON Structure

### Complete Tower Configuration Example

```json
{
  "id": "example_tower",
  "name": "Example Tower",
  "description": "A demonstration tower with all features",
  "buildable": true,
  "cost": {
    "scrap": 100,
    "petroleum": 25
  },
  "stats": {
    "damage": 10,
    "fire_rate": 1.2,
    "range": 120,
    "health": 100
  },
  "texture": {
    "width": 128,
    "height": 128,
    "base": "assets/texture/towers/example_base.png",
    "turret": "assets/texture/towers/example_turret.png"
  },
  "upgrades": {
    "max_total_upgrades": 10,
    "upgrade_types": 3,
    "1": {
      "display_name": "Rapid Fire",
      "description": "Increases fire rate by {bonus_fire_rate}.",
      "max_level": 5,
      "icon": "icons/upgrades/speed.png",
      "evolve_to": "machinegun",
      "upgrade_details": {
        "1": {
          "scrap": 50,
          "petroleum": 0,
          "bonus_stats": {
            "fire_rate": 0.2
          }
        },
        "2": {
          "scrap": 75,
          "petroleum": 25,
          "bonus_stats": {
            "fire_rate": 0.3
          }
        }
      }
    },
    "2": {
      "display_name": "Extended Range",
      "description": "Extends range by {bonus_range}.",
      "max_level": 3,
      "icon": "icons/upgrades/range.png",
      "evolve_to": "sniper",
      "upgrade_details": {
        "1": {
          "scrap": 60,
          "petroleum": 10,
          "bonus_stats": {
            "range": 30
          }
        }
      }
    }
  },
  "behaviors": {
    "combat": {
      "type": "instant",
      "targeting": "nearest",
      "effects": []
    }
  }
}
```

## Usage Examples

### Basic Tower Creation

```cpp
#include "Entity/Factory/TowerFactory.hpp"

// Create tower from JSON file
auto tower = TowerFactory::createFromConfigFile(
    "content/tower/rifle.json", 
    scene, 
    sf::Vector2f(100, 100)
);

// Access tower properties
std::cout << "Tower: " << tower->getName() << std::endl;
std::cout << "Damage: " << tower->getStat("damage", 0.0f) << std::endl;
```

### Using TowerBuilder Pattern

```cpp
// Get configured builder from JSON
TowerBuilder builder = TowerFactory::builderFromJson(jsonConfig);

// Further customize if needed
builder.setPosition(sf::Vector2f(200, 200))
       .setAngle(sf::radians(45.0f));

// Build the tower
auto tower = builder.setScene(scene).build();
```

### Working with Upgrades

```cpp
// Check upgrade availability
Currency playerCurrency(500, 100);
if (tower->canUpgrade(1, playerCurrency)) {
    // Get upgrade cost
    const auto* cost = tower->getNextUpgradeCost(1);
    std::cout << "Upgrade costs: " << cost->cost.getScraps() << " scraps" << std::endl;
    
    // Attempt upgrade
    UpgradeResult result = tower->attemptUpgrade(1, playerCurrency);
    if (result == UpgradeResult::Success) {
        std::cout << "Upgrade successful!" << std::endl;
        std::cout << "New level: " << tower->getUpgradeLevel(1) << std::endl;
    }
}

// Check for evolution opportunities
auto evolutions = tower->getAvailableEvolutions();
for (const auto& evolution : evolutions) {
    std::cout << "Can evolve to: " << evolution << std::endl;
}
```

## Key Features

### 1. Upgrade System Integration
- **JSON-driven configuration**: Define upgrade types with multiple levels
- **Flexible stat bonuses**: Support for any stat name with float values
- **Evolution system**: Towers can evolve to different types at max upgrade levels
- **Cost management**: Separate scrap and petroleum costs per upgrade level
- **Level limits**: Both per-upgrade-type and total upgrade limits

### 2. Builder Pattern Support
- **Fluent interface**: Chain method calls for easy configuration
- **JSON integration**: Automatically configure builders from JSON
- **Extensible**: Easy to add new configuration options

### 3. Validation and Error Handling
- **JSON validation**: Comprehensive checking of required and optional fields
- **Type safety**: Ensures correct data types for all fields
- **Clear error messages**: Detailed error reporting for debugging

### 4. Performance Considerations
- **Efficient parsing**: Single-pass JSON parsing with minimal allocations
- **Smart pointers**: Proper memory management throughout
- **Flexible stats**: Dynamic stat system supports any stat names

## Integration with Game Systems

### Scene Integration
```cpp
// The factory automatically handles scene integration
auto tower = TowerFactory::createFromConfigFile("tower.json", gameScene, position);
// Tower is now properly integrated with the scene's resource management
```

### Upgrade Manager
```cpp
// Access the upgrade manager for advanced operations
const auto* upgradeManager = tower->getUpgradeManager();
if (upgradeManager) {
    int totalUpgrades = upgradeManager->getTotalUpgradeCount();
    int maxUpgrades = upgradeManager->getMaxTotalUpgrades();
    
    // Get upgrade bonuses as TowerStat object
    TowerStat bonuses = upgradeManager->getAllUpgradeBonuses();
}
```

### Behavior System (Future)
The behavior parsing system is designed but not yet implemented, waiting for specific behavior classes:

```cpp
// Future implementation will support:
// - Combat behaviors (targeting, firing modes)
// - Resource behaviors (generation, collection)
// - Glowing behaviors (aura effects)
```

## Benefits

1. **Maintainability**: All tower data externalized to JSON files
2. **Flexibility**: Easy to add new tower types without code changes
3. **Balance**: Quick iteration on stats and upgrade costs
4. **Extensibility**: Ready for future behavior system integration
5. **Performance**: Efficient creation and memory management
6. **Type Safety**: Comprehensive validation and error handling

The TowerFactory provides a complete, production-ready system for tower creation with advanced upgrade support, ready for integration into the full game system.
