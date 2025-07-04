/**
 * @file SoundClickTrigger.hpp
 * @brief Declares the SoundClickTrigger class for testing sound on mouse
 * events.
 */
#pragma once

#include "Core/KeyboardObserver.hpp"
#include "Core/MouseObserver.hpp"
#include "Core/ResourceManager.hpp"
/**
 * @class SoundClickTrigger
 * @brief Test class that plays a sound when mouse events occur.
 */
class SoundClickTrigger : public MouseObserver, public KeyboardObserver {
   private:
    ResourceManager &resManager;  ///< Reference to the resource manager.
    /**
     * @brief Constructs a SoundClickTrigger with the given resource manager.
     * @param resManager Reference to the ResourceManager.
     */
   public:
    SoundClickTrigger(ResourceManager &resManager);
    /**
     * @brief Called on left mouse press.
     * @param worldPosition Mouse position in world coordinates.
     * @param windowPosition Mouse position in window coordinates.
     */
   private:
    void onLeftMousePress(const sf::Vector2f &worldPosition,
                          const sf::Vector2i &windowPosition);
    /**
     * @brief Called on right mouse press.
     * @param worldPosition Mouse position in world coordinates.
     * @param windowPosition Mouse position in window coordinates.
     */
    void onRightMousePress(const sf::Vector2f &worldPosition,
                           const sf::Vector2i &windowPosition);
    /**
     * @brief Called on left mouse release.
     * @param worldPosition Mouse position in world coordinates.
     * @param windowPosition Mouse position in window coordinates.
     */
    void onLeftMouseRelease(const sf::Vector2f &worldPosition,
                            const sf::Vector2i &windowPosition);
    /**
     * @brief Called on right mouse release.
     * @param worldPosition Mouse position in world coordinates.
     * @param windowPosition Mouse position in window coordinates.
     */
    void onRightMouseRelease(const sf::Vector2f &worldPosition,
                             const sf::Vector2i &windowPosition);

   public:
    /**
     * @brief Called when a mouse position is pressed
     * @param button The button that is pressed
     * @param event The button that user trigers
     * @param worldPosition Mouse position in world coordinates
     * @param windowPosition Mouse position in window coordinates
     */
    void onMouseEvent(Mouse button, UserEvent event,
                      const sf::Vector2f &worldPosition,
                      const sf::Vector2i &windowPosition);

    void onKeyEvent(Key key, UserEvent event, const sf::Vector2f &worldPosition,
                    const sf::Vector2i &windowPosition);
};