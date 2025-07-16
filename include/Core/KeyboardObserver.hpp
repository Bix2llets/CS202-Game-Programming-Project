/**
 * @file KeyboardObserver.hpp
 * @brief Declares the KeyboardObserver class for handling keyboard events via the observer pattern.
 *
 * The KeyboardObserver class allows objects to subscribe to and handle keyboard events (such as key presses and releases)
 * mapped to user-defined events. It works in conjunction with KeyboardState to manage subscriptions and event dispatching.
 */
#pragma once

#include <SFML/Graphics.hpp>
class KeyboardState;
enum class Key;
enum class UserEvent;

/**
 * @class KeyboardObserver
 * @brief Abstract base class for objects that observe keyboard events.
 *
 * KeyboardObserver provides an interface for subscribing to, unsubscribing from, and handling keyboard events.
 * Derived classes should implement the onKeyEvent method to define custom behavior for specific key events.
 */
class KeyboardObserver {
    public:
    /**
     * @brief Subscribe to a specific key and user event combination.
     * @param key The key to observe.
     * @param event The user-defined event (e.g., Press, Release).
     * @param state Reference to the KeyboardState managing subscriptions.
     */
    inline void subscribeKeyboard(Key key, UserEvent event, KeyboardState& state);

    /**
     * @brief Unsubscribe from a specific key and user event combination.
     * @param key The key to stop observing.
     * @param event The user-defined event.
     * @param state Reference to the KeyboardState managing subscriptions.
     */
    inline void unSubscribeKeyboard(Key key, UserEvent event, KeyboardState& state);

    /**
     * @brief Handle a keyboard event. Should be overridden by derived classes.
     * @param key The key involved in the event.
     * @param event The user-defined event (e.g., Press, Release).
     * @param worldPosition The world coordinates of the event (based on mouse position, if relevant).
     * @param windowPosition The window coordinates of the event (based on mouse position, if relevant).
     */
    virtual void onKeyEvent(Key key, UserEvent event, const sf::Vector2f &worldPosition,
                    const sf::Vector2f &windowPosition) = 0;
};