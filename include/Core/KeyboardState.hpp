/**
 * @file KeyboardState.hpp
 * @brief Declares the KeyboardState class and related enums for keyboard event management.
 *
 * This file defines the Key enum for user-defined key codes and the KeyboardState class,
 * which manages keyboard event subscriptions and dispatches events to observers.
 */
#pragma once

#include <SFML/Graphics.hpp>
#include "Core/UserEvent.hpp"

#include <optional>
#include <map>
#include <list>
class KeyboardObserver;

/**
 * @enum Key
 * @brief User-defined key codes for keyboard input abstraction.
 *
 * This enum provides a mapping for all supported keyboard keys, including letters, numbers,
 * function keys, and special keys. Used for decoupling SFML key codes from application logic.
 */
enum class Key {
    Unknown,
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
    Escape, LControl, LShift, LAlt, LSystem,
    RControl, RShift, RAlt, RSystem,
    Menu, LBracket, RBracket, Semicolon, Comma, Period, Apostrophe, Slash, Backslash,
    Grave, Equal, Hyphen, Space, Enter, Backspace, Tab, PageUp, PageDown,
    End, Home, Insert, Delete, Add, Subtract, Multiply, Divide,
    Left, Right, Up, Down,
    Numpad0, Numpad1, Numpad2, Numpad3, Numpad4,
    Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,
    Pause,
    KeyCount // Keep this last
};

/**
 * @class KeyboardState
 * @brief Manages keyboard event subscriptions and dispatching using the observer pattern.
 *
 * KeyboardState tracks which observers are interested in which key and user event combinations.
 * It provides methods to add/remove/clear subscribers and to handle SFML events, dispatching them
 * to the appropriate observers.
 */
class KeyboardState {

    private: 
    std::map<Key, std::map<UserEvent, std::list<KeyboardObserver*>>> subscriberList;    ///< Subscription map.
    public:
    /**
     * @brief Construct a KeyboardState for a given window.
     * @param window Reference to the SFML RenderWindow.
     */
    KeyboardState();

    /**
     * @brief Add an observer for a specific key and user event.
     * @param key The key to observe.
     * @param event The user-defined event.
     * @param subscriber Pointer to the observer.
     */
    void addSubscriber(Key key, UserEvent event, KeyboardObserver* subscriber);

    /**
     * @brief Remove an observer for a specific key and user event.
     * @param key The key to stop observing.
     * @param event The user-defined event.
     * @param subscriber Pointer to the observer.
     */
    void removeSubscriber(Key key, UserEvent event, KeyboardObserver* subscriber);

    /**
     * @brief Remove all observers for a specific key and user event.
     * @param key The key to clear.
     * @param event The user-defined event.
     */
    void clearSubscriber(Key key, UserEvent event);

    /**
     * @brief Remove all observers for all keys and events.
     */
    void clearSubscriber();

    /**
     * @brief Handle an SFML event, dispatching to observers as appropriate.
     * @param event The SFML event to process (optional).
     */
    void handleEvent(std::optional<sf::Event> &event);

private:
    sf::Vector2f scalePosition(sf::Vector2f input);
};