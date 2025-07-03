/**
 * @file EntityUsageExample.cpp
 * @brief Example showing how to properly use the Entity system with sprites
 */

#include "EntitySystem.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

void demonstrateEntityWithSprites() {
    
}

// Example of rendering entities (would be called in main game loop)
void renderEntities(sf::RenderWindow& window, const std::vector<std::unique_ptr<Entity>>& entities) {
    for (const auto& entity : entities) {
        if (entity && entity->isAlive()) {
            entity->render(window);
        }
    }
}

int main() {
    try {
        demonstrateEntityWithSprites();
        
        // Example of how to use in a game loop context
        std::cout << "\nExample game loop integration:" << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
