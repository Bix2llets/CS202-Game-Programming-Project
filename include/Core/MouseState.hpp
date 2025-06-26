/**
 * @file MouseState.hpp
 * @brief Defines the MouseState class for managing mouse button event
 * subscriptions and notifications.
 *
 * This class allows observers to subscribe to mouse button events (left or
 * right) and be notified when those events occur.
 *
 * Dependencies: SFML/Graphics.hpp, <map>, <optional>, <list>, and a forward
 * declaration of MouseObserver.
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include <map>
#include <optional>

// Forward declaration to break circular dependency
class MouseObserver;

/**
 * @enum MouseButton
 * @brief Enum representing mouse buttons that can be observed.
 */
enum class MouseButton { Left, Right };
enum class MouseEvent { Release, Click };

/**
 * @class MouseState
 * @brief Manages mouse button event subscriptions and notifies observers.
 *]
 * MouseState allows objects to subscribe to specific mouse button events and be
 *notified when those events occur. Observers are managed as pointers and
 *grouped by mouse button.
 */
class MouseState {
   private:
    sf::RenderWindow &window;
    /**
     * @brief Maps each mouse button to a list of pointers to MouseObserver
     * objects that are subscribed to that button's events.
     */
    std::map<MouseButton, std::map<MouseEvent, std::list<MouseObserver *>>>
        subscriberList;

   public:
    MouseState(sf::RenderWindow &window);
    /**
     * @brief Handles an SFML event and notifies relevant observers if it is a
     * mouse button event.
     * @param event The SFML event to handle (optional).
     */
    void handleEvent(const std::optional<sf::Event> &event);

    /**
     * @brief Adds a MouseObserver pointer to the subscriber list for the
     * specified mouse button.
     * @param button The mouse button to subscribe to.
     * @param subscriber Pointer to the MouseObserver to add.
     */
    void addSubscriber(MouseButton button, MouseEvent event,
                       MouseObserver *subscriber);

    /**
     * @brief Removes a MouseObserver pointer from the subscriber list for the
     * specified mouse button.
     * @param button The mouse button to unsubscribe from.
     * @param subscriber Pointer to the MouseObserver to remove.
     */
    void removeSubscriber(MouseButton button, MouseEvent event,
                          MouseObserver *subscriber);

    /**
     * @brief Removes all subscribers from all mouse buttons.
     */
    void clearSubscriber();

    /**
     * @brief Removes all subscribers from a specific mouse button.
     * @param button The mouse button whose subscribers will be cleared.
     */
    void clearSubscriber(MouseButton button, MouseEvent event);
};