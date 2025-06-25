/**
 * @file SoundClickTrigger.hpp
 * @brief Declares the SoundClickTrigger class for testing sound on mouse events.
 */
#pragma once

#include "Core/MouseObserver.hpp"
#include "Core/ResourceManager.hpp"

/**
 * @class SoundClickTrigger
 * @brief Test class that plays a sound when mouse events occur.
 */
class SoundClickTrigger : public MouseObserver{
    ResourceManager &resManager; ///< Reference to the resource manager.
    public:
    /**
     * @brief Constructs a SoundClickTrigger with the given resource manager.
     * @param resManager Reference to the ResourceManager.
     */
    SoundClickTrigger(ResourceManager &resManager);
    /**
     * @brief Called on left mouse click.
     * @param worldPosition Mouse position in world coordinates.
     * @param windowPosition Mouse position in window coordinates.
     */
    void onLeftMouseClick(const sf::Vector2f &worldPosition, const sf::Vector2i& windowPosition);
    /**
     * @brief Called on right mouse click.
     * @param worldPosition Mouse position in world coordinates.
     * @param windowPosition Mouse position in window coordinates.
     */
    void onRightMouseClick(const sf::Vector2f &worldPosition, const sf::Vector2i& windowPosition);
    /**
     * @brief Called on left mouse release.
     * @param worldPosition Mouse position in world coordinates.
     * @param windowPosition Mouse position in window coordinates.
     */
    void onLeftMouseRelease(const sf::Vector2f &worldPosition, const sf::Vector2i& windowPosition);
    /**
     * @brief Called on right mouse release.
     * @param worldPosition Mouse position in world coordinates.
     * @param windowPosition Mouse position in window coordinates.
     */
    void onRightMouseRelease(const sf::Vector2f &worldPosition, const sf::Vector2i& windowPosition);
};