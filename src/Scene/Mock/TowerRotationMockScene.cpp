#include "Scene/Mock/TowerRotationMockScene.hpp"
#include "Entity/Tower/TowerStat.hpp"
#include "Entity/Factory/TowerFactory.hpp"
#include "Gameplay/Currency.hpp"
#include <cmath>
#include <iostream>

TowerRotationMockScene::TowerRotationMockScene(sf::RenderWindow& window,
                                              SceneManager& parentManager, InputManager& inputManager, 
                                              ResourceManager& resourceManager, JSONLoader &loader)
    : Scene(window, parentManager, inputManager, resourceManager, loader),
      rotationSpeed(1.0f), // 1 radian per second (slow rotation)
      font(nullptr) {
    initialize();
}

void TowerRotationMockScene::initialize() {
    createTestTower();
    setupInfoText();
}

void TowerRotationMockScene::createTestTower() {
    // Get window center position dynamically
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f centerPos(windowSize.x / 2.0f, windowSize.y / 2.0f);

    std::cout << "Tower will be positioned at: " << centerPos.x << ", " << centerPos.y << std::endl;
    
    try {
        // Test TowerFactory: Create tower from JSON configuration
        std::string configPath = "content/tower/rifle.json";
        testTower = TowerFactory::createFromConfigFile(configPath, *this, centerPos);
        
        std::cout << "Successfully created tower from JSON: " << testTower->getName() << std::endl;
        std::cout << "Tower stats - Damage: " << testTower->getStat("damage") 
                  << ", Range: " << testTower->getStat("range") 
                  << ", Fire Rate: " << testTower->getStat("fire_rate") << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Failed to load tower from JSON: " << e.what() << std::endl;
        std::cout << "Falling back to TowerBuilder..." << std::endl;
        
        // Fallback: Create tower using TowerBuilder (original method)
        auto stats = std::make_unique<TowerStat>();
        stats->setStat("damage", 10.0f);
        stats->setStat("fire_rate", 1.2f);
        stats->setStat("range", 120.0f);
        
        TowerBuilder builder;
        testTower = builder
            .setId("mock_rifle")
            .setScene(*this)
            .setName("Mock Rifle Tower (Fallback)")
            .setDescription("A test rifle tower created with TowerBuilder")
            .setBuildable(true)
            .setCost(Currency(100, 0))
            .setPosition(centerPos)
            .setAngle(sf::radians(0.f))
            .setStats(std::move(stats))
            .setTimerInterval(1.0f / 1.2f)
            .build();
    }
    
    // Note: In a real implementation, you would load actual textures here
    // For now, the tower will be created without textures for testing the rotation logic
}

void TowerRotationMockScene::setupInfoText() {
    // Get font from resource manager
    font = const_cast<sf::Font*>(getFont("LeagueSpartan"));
    
    if (font) {
        infoText = std::make_unique<sf::Text>(*font);
        infoText->setCharacterSize(24);
        infoText->setFillColor(sf::Color::White);
        infoText->setPosition(sf::Vector2f(10, 10));
        
        // Set initial text content
        std::string info = "Tower Rotation Mock Scene\n";
        info += "Tower: " + testTower->getName() + "\n";
        info += "ID: " + testTower->getId() + "\n";
        info += "Position: (" + std::to_string(static_cast<int>(testTower->getPosition().x)) + 
               ", " + std::to_string(static_cast<int>(testTower->getPosition().y)) + ")\n";
        info += "Damage: " + std::to_string(static_cast<int>(testTower->getStat("damage"))) + "\n";
        info += "Range: " + std::to_string(static_cast<int>(testTower->getStat("range"))) + "\n";
        info += "Fire Rate: " + std::to_string(testTower->getStat("fire_rate")) + "\n";
        info += "Cost: " + std::to_string(testTower->getCost().getScraps()) + " scraps\n";
        info += "\nTurret rotates slowly to test dual-sprite system";
        
        infoText->setString(info);
    }
}

void TowerRotationMockScene::update() {
    if (testTower) {
        updateTowerRotation();
        testTower->update();
    }
}

void TowerRotationMockScene::updateTowerRotation() {
    // Calculate rotation based on elapsed time
    float deltaTime = rotationClock.restart().asSeconds();
    
    // Get current turret rotation and add rotation increment
    sf::Angle currentRotation = testTower->getTurretRotation();
    sf::Angle newRotation = currentRotation + sf::radians(rotationSpeed * deltaTime);
    
    // Apply the new rotation to the turret only (base stays fixed)
    testTower->setTurretRotation(newRotation);
    
    // Update info text with current rotation (optional, for debugging)
    if (font && infoText) {
        float rotationDegrees = newRotation.asDegrees();
        
        std::string info = "Tower Rotation Mock Scene\n";
        info += "Tower: " + testTower->getName() + "\n";
        info += "ID: " + testTower->getId() + "\n";
        info += "Position: (" + std::to_string(static_cast<int>(testTower->getPosition().x)) + 
               ", " + std::to_string(static_cast<int>(testTower->getPosition().y)) + ")\n";
        info += "Damage: " + std::to_string(static_cast<int>(testTower->getStat("damage"))) + "\n";
        info += "Range: " + std::to_string(static_cast<int>(testTower->getStat("range"))) + "\n";
        info += "Fire Rate: " + std::to_string(testTower->getStat("fire_rate")) + "\n";
        info += "Cost: " + std::to_string(testTower->getCost().getScraps()) + " scraps\n";
        info += "\nTurret Rotation: " + std::to_string(static_cast<int>(rotationDegrees)) + "°";
        info += "\nBase Rotation: 0° (fixed)";
        
        // Add texture loading status
        std::string textureStatus = "\nTextures: ";
        textureStatus += "Loaded (Base & Turret)";
        textureStatus += "\nSize: " + std::to_string(static_cast<int>(testTower->getTextureWidth())) + 
                        "x" + std::to_string(static_cast<int>(testTower->getTextureHeight()));
        info += textureStatus;
        
        infoText->setString(info);
    }
}

void TowerRotationMockScene::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    // Draw the tower using its own draw method (textures from JSON if loaded)
    if (testTower) {
        target.draw(*testTower, state);
    }
    
    // Draw info text overlay
    if (infoText) {
        target.draw(*infoText, state);
    }
}

void TowerRotationMockScene::registerComponents() {
    // No interactive components in this mock scene
}

void TowerRotationMockScene::unRegisterComponents() {
    // No interactive components to unregister
}
