/**
 * @file Scene.hpp
 * @brief Declares the abstract Scene class, the base for all game scenes.
 */
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>
#include <string>

#include "GUIComponents/Mediator.hpp"
/**
 * @class Scene
 * @brief Abstract base class for all game scenes.
 *
 * Scenes represent different states or screens in the game (e.g., menu,
 * gameplay).
 */
class Scene : public sf::Drawable, public Mediator {
   protected:

   public:
    Scene() = default;
    /**
     * @brief Draws the scene (pure virtual).
     * @param target The render target to draw to.
     * @param state Current render states.
     */
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates state) const = 0;
    /**
     * @brief Updates the scene (pure virtual).
     */
    virtual void update() = 0;
    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    virtual ~Scene() = default;

    /**
     * @brief Register all GUI components to the inputManager.
     */
    virtual void onLoad() = 0;
    /**
     * @brief un-register all GUI components to the inputManager.
     */
    virtual void onUnload() = 0;
};