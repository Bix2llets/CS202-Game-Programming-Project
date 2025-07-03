#include "EntitySystem.hpp"
#include <iostream>
#include <vector>
#include <memory>

/**
 * @brief Example demonstrating the Entity System usage
 * 
 * This example shows how to:
 * - Create different types of entities
 * - Use the Strategy pattern for attacks
 * - Use the State pattern for enemy behavior
 * - Manage entity interactions
 */
void demonstrateEntitySystem() {
    
}

int main() {
    try {
        demonstrateEntitySystem();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
