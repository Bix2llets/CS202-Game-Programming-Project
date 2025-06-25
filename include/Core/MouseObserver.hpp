/**
 * @file MouseObserver.hpp
 * @brief Interface for objects that observe mouse button events via MouseState.
 *
 * This class provides an interface for subscribing and unsubscribing to mouse button events managed by MouseState.
 *
 * Forward declarations are used to avoid circular dependencies.
 */
#pragma once

#include <SFML/Graphics.hpp>
// Forward declarations to avoid circular dependency
class MouseState;
/**
 * @enum MouseButton
 * @brief Enum representing mouse buttons that can be observed.
 */
enum class MouseButton;
/**
 * @enum MouseEvent
 * @brief Enum representing mouse events (click, release, etc.) that can be observed.
 */
enum class MouseEvent;
/**
 * @class MouseObserver
 * @brief Abstract interface for mouse event observers.
 *
 * Classes inheriting from MouseObserver can subscribe to mouse button events managed by MouseState.
 * They must implement the mouse event handler methods for click and release events.
 */
class MouseObserver {
   public:
    /**
     * @brief Subscribe this observer to a mouse button event in the given MouseState.
     * @param button The mouse button to observe.
     * @param event The mouse event type to observe (click, release, etc.).
     * @param mouseState The MouseState to subscribe to.
     */
    virtual void subscribe(MouseButton button, MouseEvent event, MouseState& mouseState);
    /**
     * @brief Unsubscribe this observer from a mouse button event in the given MouseState.
     * @param button The mouse button to stop observing.
     * @param event The mouse event type to stop observing.
     * @param mouseState The MouseState to unsubscribe from.
     */
    virtual void unSubscribe(MouseButton button, MouseEvent event, MouseState& mouseState);
    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    virtual ~MouseObserver() = default;
    /**
     * @brief Called when the left mouse button is clicked.
     * @param worldPosition The mouse position in world coordinates.
     * @param windowPosition The mouse position in window coordinates.
     */
    virtual void onLeftMouseClick(const sf::Vector2f &worldPosition, const sf::Vector2i &windowPosition) = 0;
    /**
     * @brief Called when the right mouse button is clicked.
     * @param worldPosition The mouse position in world coordinates.
     * @param windowPosition The mouse position in window coordinates.
     */
    virtual void onRightMouseClick(const sf::Vector2f &worldPosition, const sf::Vector2i &windowPosition) = 0;
    /**
     * @brief Called when the left mouse button is released.
     * @param worldPosition The mouse position in world coordinates.
     * @param windowPosition The mouse position in window coordinates.
     */
    virtual void onLeftMouseRelease(const sf::Vector2f &worldPosition, const sf::Vector2i &windowPosition) = 0;
    /**
     * @brief Called when the right mouse button is released.
     * @param worldPosition The mouse position in world coordinates.
     * @param windowPosition The mouse position in window coordinates.
     */
    virtual void onRightMouseRelease(const sf::Vector2f &worldPosition, const sf::Vector2i &windowPosition) = 0;
};