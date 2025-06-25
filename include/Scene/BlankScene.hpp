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
     * @param window Reference to the SFML render window.
     * @param name Name of the scene.
     */
    BlankScene(sf::RenderWindow &window, const std::string &name) : Scene{window, name} {}
    /**
     * @brief Handles an event (no-op for blank scene).
     */
    void handleEvent(std::optional<sf::Event> & event) {}
    /**
     * @brief Handles input (no-op for blank scene).
     */
    void handleInput() {}
    /**
     * @brief Draws the scene (no-op for blank scene).
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
};