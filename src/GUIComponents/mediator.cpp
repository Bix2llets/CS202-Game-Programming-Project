#include "GUIComponents/mediator.hpp"

void Mediator::subscribe(const std::string& event, Handler handler) {
    handlers[event].push_back(std::move(handler));
}

void Mediator::notify(const std::string& event, std::any sender, std::any data) {
    // Add event to queue instead of immediately processing
    eventQueue.emplace(event, std::move(sender), std::move(data));
}

void Mediator::resolveQueue() {
    // Process all events in the queue
    while (!eventQueue.empty()) {
        const QueuedEvent& queuedEvent = eventQueue.front();
        
        // Find and execute handlers for this event
        auto it = handlers.find(queuedEvent.event);
        if (it != handlers.end()) {
            for (auto& handler : it->second) {
                handler(queuedEvent.sender, queuedEvent.data);
            }
        }
        
        eventQueue.pop();
    }
}

void Mediator::notifyImmediate(const std::string& event, std::any sender, std::any data) {
    // Immediate execution without queueing (legacy behavior)
    auto it = handlers.find(event);
    if (it != handlers.end()) {
        for (auto& handler : it->second) {
            handler(sender, data);
        }
    }
}

bool Mediator::hasQueuedEvents() const {
    return !eventQueue.empty();
}

void Mediator::clearQueue() {
    std::queue<QueuedEvent> empty;
    eventQueue.swap(empty); // Clear the queue
}
