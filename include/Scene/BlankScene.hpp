/**
 * @file BlankScene.hpp
 * @brief Declares the BlankScene class for unit testing purposes.
 */
#pragma once
#include "Scene/Scene.hpp"

/**
 * @class BlankScene
 * @brief Minimal scene implementation for unit testing.
 */
class BlankScene : public Scene {
   public:
    /**
     * @brief Constructs a BlankScene with the given window and name.
     * @param name Name of the scene.
     * @param parentManager Reference to the parent SceneManager.
     * @param resManager Reference to the ResourceManager singleton.
     */
    BlankScene(SceneManager &parentManager,  
               ResourceManager &resManager)
        : Scene{parentManager, resManager} {}
    /**
     * @brief Draws the scene (no-op for blank scene).
     * @param target The render target to draw to.
     * @param state Current render states.
     */
    void draw(sf::RenderTarget &target, sf::RenderStates state) const {}
    /**
     * @brief Updates the scene (no-op for blank scene).
     */
    void update() {}
    /**
     * @brief Destructor.
     */
    ~BlankScene() = default;

    void onLoad() override {
        // No components to register in a blank scene
    };
    void onUnload() override {
        // No components to unregister in a blank scene
    };
};