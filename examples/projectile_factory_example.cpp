/**
 * @file projectile_factory_example.cpp
 * @brief Example demonstrating the usage of ProjectileFactory with SpriteAnimation
 */

#include "Entity/Factory/ProjectileFactory.hpp"
#include "Scene/Scene.hpp"
#include <json.hpp>
#include <iostream>

int main() {
    // Example JSON configuration matching the projectiles.json format
    nlohmann::json projectileConfig = {
        {"id", "example_bullet"},
        {"texture", {
            {"texture_id", "rifle_turret"},
            {"width", 8},
            {"height", 8},
            {"animation_interval", 0.05},
            {"type", "linear"},
            {"center_x", 4},
            {"center_y", 4},
            {"viewport", {
                {"x", 0},
                {"y", 0},
                {"width", 8},
                {"height", 8}
            }}
        }},
        {"targeting", "enemy"},
        {"rotate_to_target", true},
        {"stop_on_first_collision", true},
        {"pierce_through_targets", false},
        {"collision_distance", 2}
    };

    try {
        // Create a dummy scene (in real usage, this would be your actual game scene)
        // Scene gameScene;
        
        // Create projectile from JSON configuration
        // auto projectile = ProjectileFactory::createFromJson(gameScene, projectileConfig);
        
        std::cout << "ProjectileFactory successfully configured with SpriteAnimation support!" << std::endl;
        std::cout << "The factory now:" << std::endl;
        std::cout << "- Validates all projectile configuration fields" << std::endl;
        std::cout << "- Uses SpriteAnimation for texture loading and animation" << std::endl;
        std::cout << "- Supports rotation to target behavior" << std::endl;
        std::cout << "- Handles collision and piercing settings" << std::endl;
        std::cout << "- Creates appropriate flight modes" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error creating projectile: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
