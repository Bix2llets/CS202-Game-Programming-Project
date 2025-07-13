/**
 * @file TowerRotationMockScene.hpp
 * @brief Mock scene for testing TowerBuilder and tower rotation functionality.
 *
 * This scene demonstrates:
 * - Creating a tower using TowerBuilder (not JSON)
 * - Tower dual-sprite system (base and turret)
 * - Turret rotation while base remains fixed
 * - Basic tower rendering and update loop
 */
#pragma once

#include <memory>
#include <optional>
#include <SFML/Graphics.hpp>

#include "Scene/Scene.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/Tower/TowerBuilder.hpp"
#include "Core/InputManager.hpp"
#include "Core/ResourceManager.hpp"

/**
 * @class TowerRotationMockScene
 * @brief Mock scene for testing tower creation and rotation.
 *
 * Creates a rifle tower using TowerBuilder and continuously rotates its turret
 * to demonstrate the dual-sprite system and rotation functionality.
 */
class TowerRotationMockScene : public Scene {
private:
    std::unique_ptr<Tower> testTower;    ///< The test tower created with TowerBuilder
    sf::Clock rotationClock;             ///< Clock for rotation timing
    float rotationSpeed;                 ///< Speed of turret rotation (radians per second)
    
    // UI elements for display
    std::unique_ptr<sf::Text> infoText;  ///< Information text overlay (created when font is available)
    sf::Font* font;                      ///< Font for text rendering

public:
    /**
     * @brief Construct a new TowerRotationMockScene.
     * @param window Reference to the render window.
     * @param name Name of the scene.
     * @param parentManager Reference to the scene manager.
     * @param inputManager Reference to the input manager.
     * @param resourceManager Reference to the resource manager.
     */
    TowerRotationMockScene(sf::RenderWindow& window,
                          SceneManager& parentManager, InputManager& inputManager, 
                          ResourceManager& resourceManager, JSONLoader &loader);
    
    /**
     * @brief Destructor.
     */
    ~TowerRotationMockScene() override = default;
    
    /**
     * @brief Initialize the scene - create the test tower.
     */
    void initialize();
    
    /**
     * @brief Update the scene - rotate the turret and update tower.
     */
    void update() override;
    
    /**
     * @brief Render the scene - draw tower and info text.
     * @param target The render target to draw to.
     * @param state Current render states.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
    
    /**
     * @brief Register components to input manager.
     */
    void registerComponents() override;
    
    /**
     * @brief Unregister components from input manager.
     */
    void unRegisterComponents() override;

private:
    /**
     * @brief Create the test tower using TowerBuilder.
     */
    void createTestTower();
    
    /**
     * @brief Setup the info text overlay.
     */
    void setupInfoText();
    
    /**
     * @brief Update the rotation of the tower's turret.
     */
    void updateTowerRotation();
};
