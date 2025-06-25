/**
 * @file GameObject.hpp
 * @brief Declares the GameObject abstract base class for drawable and updatable game objects.
 */
#pragma once
#include <SFML/Graphics.hpp>
#include "Base/Constants.hpp"
/**
 * @class GameObject
 * @brief Abstract base class for all drawable and updatable game objects.
 *
 * Inherit from this class to create objects that can be drawn and updated in the game loop.
 */
class GameObject : public sf::Drawable {
    /**
     * @brief Draw the object to the given render target.
     * @param target The render target.
     * @param state The render states.
     */
    virtual void draw(sf::RenderTarget &target, sf::RenderStates state) const = 0;
    /**
     * @brief Update the object's state.
     */
    virtual void update() const = 0;
    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    virtual ~GameObject() = default;
};