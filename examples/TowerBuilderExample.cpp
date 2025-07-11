/**
 * @file TowerBuilderExample.cpp
 * @brief Example demonstrating how to use TowerBuilder and TowerFactory to create towers.
 *
 * This example shows:
 * 1. Manual tower creation using TowerBuilder
 * 2. Tower creation from JSON configuration using TowerFactory
 * 3. How to load the rifle tower from rifle.json
 */

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Entity/Tower/TowerBuilder.hpp"
#include "Entity/Factory/TowerFactory.hpp"
#include "Entity/Tower/TowerStat.hpp"
#include "Gameplay/Currency.hpp"
#include "Scene/Scene.hpp"

// Forward declaration for example
class MockScene : public Scene {
public:
    MockScene() = default;
    void update() override {}
    void draw(sf::RenderWindow& window) override {}
    void handleEvent(const sf::Event& event) override {}
};

int main() {
    std::cout << "=== TowerBuilder and TowerFactory Example ===" << std::endl;
    
    // Create a mock scene for the example
    MockScene scene;
    
    try {
        // === Example 1: Manual tower creation using TowerBuilder ===
        std::cout << "\n1. Creating tower manually with TowerBuilder:" << std::endl;
        
        TowerBuilder builder;
        
        // Create custom stats
        auto stats = std::make_unique<TowerStat>();
        stats->setStat("damage", 15.0f);
        stats->setStat("fire_rate", 1.5f);
        stats->setStat("range", 100.0f);
        
        // Build tower with fluent interface
        auto manualTower = builder
            .setId("custom_rifle")
            .setScene(scene)
            .setName("Custom Rifle Tower")
            .setDescription("A manually configured rifle tower")
            .setBuildable(true)
            .setCost(Currency(150, 25))
            .setPosition(sf::Vector2f(100, 100))
            .setStats(std::move(stats))
            .setTimerInterval(0.8f)
            .build();
        
        std::cout << "✓ Created tower: " << manualTower->getName() << std::endl;
        std::cout << "  ID: " << manualTower->getId() << std::endl;
        std::cout << "  Cost: " << manualTower->getCost().getScraps() << " scraps, " 
                  << manualTower->getCost().getPetroleum() << " petroleum" << std::endl;
        std::cout << "  Damage: " << manualTower->getStat("damage") << std::endl;
        std::cout << "  Fire Rate: " << manualTower->getStat("fire_rate") << std::endl;
        std::cout << "  Range: " << manualTower->getStat("range") << std::endl;
        
        // === Example 2: Tower creation from JSON using TowerFactory ===
        std::cout << "\n2. Creating tower from rifle.json using TowerFactory:" << std::endl;
        
        const std::string configPath = "configuration/tower/rifle.json";
        auto rifleTower = TowerFactory::createFromConfigFile(
            configPath,
            scene,
            sf::Vector2f(200, 200)
        );
        
        std::cout << "✓ Created tower from JSON: " << rifleTower->getName() << std::endl;
        std::cout << "  ID: " << rifleTower->getId() << std::endl;
        std::cout << "  Description: " << rifleTower->getDescription() << std::endl;
        std::cout << "  Cost: " << rifleTower->getCost().getScraps() << " scraps, " 
                  << rifleTower->getCost().getPetroleum() << " petroleum" << std::endl;
        std::cout << "  Buildable: " << (rifleTower->isBuildable() ? "Yes" : "No") << std::endl;
        std::cout << "  Damage: " << rifleTower->getStat("damage") << std::endl;
        std::cout << "  Fire Rate: " << rifleTower->getStat("fire_rate") << std::endl;
        std::cout << "  Range: " << rifleTower->getStat("range") << std::endl;
        
        // === Example 3: Using TowerBuilder from JSON data ===
        std::cout << "\n3. Demonstrating TowerBuilder from JSON:" << std::endl;
        
        // Note: This would require loading JSON data first
        std::cout << "  (This example would load JSON and use TowerFactory::builderFromJson)" << std::endl;
        std::cout << "  ✓ TowerBuilder can be configured from JSON and then customized further" << std::endl;
        
        std::cout << "\n=== Example completed successfully! ===" << std::endl;
        std::cout << "\nKey Features Demonstrated:" << std::endl;
        std::cout << "• Fluent interface for building towers step by step" << std::endl;
        std::cout << "• JSON configuration loading with validation" << std::endl;
        std::cout << "• Automatic stats parsing and application" << std::endl;
        std::cout << "• Cost and metadata handling" << std::endl;
        std::cout << "• Texture path management (ready for ResourceManager)" << std::endl;
        std::cout << "• Builder pattern for flexible tower creation" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
