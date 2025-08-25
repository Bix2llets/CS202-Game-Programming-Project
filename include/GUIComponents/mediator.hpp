#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <queue>
#include <any>

#include <SFML/Graphics.hpp>
#include <string> 

/**
 * @class Mediator
 * @brief Generic mediator for UI components using the mediator pattern.
 *
 * The Mediator class allows components to subscribe to and notify named events.
 * Handlers can be registered for specific event names, and events can carry sender and data payloads.
 * Events are queued when notified and processed when resolveQueue() is called.
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
     * @brief Structure representing a queued event.
     */
    struct QueuedEvent {
        std::string event;
        std::any sender;
        std::any data;
        
        QueuedEvent(const std::string& e, std::any s, std::any d)
            : event(e), sender(std::move(s)), data(std::move(d)) {}
    };

    /**
     * @brief Subscribes a handler to a specific event name.
     * @param event The event name to subscribe to.
     * @param handler The handler function to call when the event is notified.
     */
    void subscribe(const std::string& event, Handler handler);

    /**
     * @brief Queues an event for later processing.
     * @param event The event name to notify.
     * @param sender The sender of the event (optional).
     * @param data Optional event data.
     */
    void notify(const std::string& event, std::any sender = {}, std::any data = {}); 

    /**
     * @brief Processes all queued events in order.
     * This should be called at appropriate times (e.g., end of frame) to handle all pending events.
     */
    void resolveQueue();

    /**
     * @brief Immediately executes handlers for an event without queueing.
     * @param event The event name to notify.
     * @param sender The sender of the event (optional).
     * @param data Optional event data.
     */
    void notifyImmediate(const std::string& event, std::any sender = {}, std::any data = {});

    /**
     * @brief Checks if there are any events in the queue.
     * @return True if there are queued events, false otherwise.
     */
    bool hasQueuedEvents() const;

    /**
     * @brief Clears all queued events without processing them.
     */
    void clearQueue();

private:
    std::unordered_map<std::string, std::vector<Handler>> handlers;
    std::queue<QueuedEvent> eventQueue;
};
