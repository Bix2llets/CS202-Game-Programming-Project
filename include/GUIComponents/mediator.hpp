#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <any>

#include <SFML/Graphics.hpp>
#include <string> 
class Button; // Forward declaration

/**
 * @class Mediator
 * @brief Generic mediator for UI components using the mediator pattern.
 *
 * The Mediator class allows components to subscribe to and notify named events.
 * Handlers can be registered for specific event names, and events can carry sender and data payloads.
 */
class Mediator {
public:
    /**
     * @brief Handler type for event callbacks.
     * @param sender The sender of the event (as std::any).
     * @param data Optional event data (as std::any).
     */
    using Handler = std::function<void(std::any sender, std::any data)>;

    /**
     * @brief Subscribes a handler to a specific event name.
     * @param event The event name to subscribe to.
     * @param handler The handler function to call when the event is notified.
     */
    void subscribe(const std::string& event, Handler handler);

    /**
     * @brief Notifies all handlers of an event.
     * @param event The event name to notify.
     * @param sender The sender of the event (optional).
     * @param data Optional event data.
     */
    void notify(const std::string& event, std::any sender = {}, std::any data = {}); 

    /**
     * @brief Get font for rendering use (should be reasonable as mediator stores UI component, which need font to display text)
     * @param fontName the name of the font
     */

     virtual const sf::Font* const getFont(std::string fontName) = 0;
private:
    std::unordered_map<std::string, std::vector<Handler>> handlers;
};
