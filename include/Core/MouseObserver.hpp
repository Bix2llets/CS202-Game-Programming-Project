/**
 * @file MouseObserver.hpp
 * @brief Interface for objects that observe mouse button events via MouseState.
 *
 * This class provides an interface for subscribing and unsubscribing to mouse
 * button events managed by MouseState.
 *
 * Forward declarations are used to avoid circular dependencies.
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <unordered_map>
// Forward declarations to avoid circular dependency
class MouseState;
/**
 * @enum Mouse
 * @brief Enum representing mouse buttons that can be observed.
 */
enum class Mouse;
/**
 * @enum UserEvent
 * @brief Enum representing mouse events (press, release, etc.) that can be
 * observed.
 */
enum class UserEvent;
/**
 * @class MouseObserver
 * @brief Abstract interface for mouse event observers.
 *
 * Classes inheriting from MouseObserver can subscribe to mouse button events
 * managed by MouseState. They must implement the mouse event handler methods
 * for press and release events.
 */
class MouseObserver {
   public:
    /**
     * @brief Subscribe this observer to a mouse button event in the given
     * MouseState.
     * @param button The mouse button to observe.
     * @param event The mouse event type to observe (press, release, etc.).
     * @param mouseState The MouseState to subscribe to.
     */
    virtual void subscribeMouse(Mouse button, UserEvent event,
                           MouseState& mouseState);
    /**
     * @brief Unsubscribe this observer from a mouse button event in the given
     * MouseState.
     * @param button The mouse button to stop observing.
     * @param event The mouse event type to stop observing.
     * @param mouseState The MouseState to unsubscribe from.
     */
    virtual void unSubscribeMouse(Mouse button, UserEvent event,
                             MouseState& mouseState);
    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    virtual ~MouseObserver() = default;
    /**
     * @brief Called when a mouse button is pressed.
     * @param button The button that is pressed
     * @param event The button that user trigers
     * @param worldPosition The mouse position in world coordinates.
     * @param windowPosition The mouse position in window coordinates.
     */
    virtual void onMouseEvent(Mouse button, UserEvent event,
                              const sf::Vector2f& worldPosition,
                              const sf::Vector2i& windowPosition) = 0;
};