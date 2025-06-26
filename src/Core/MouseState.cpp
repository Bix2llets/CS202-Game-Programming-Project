#include "Core/MouseState.hpp"

#include <algorithm>
#include <list>
#include <sstream>
#include <utility>

#include "Core/MouseObserver.hpp"
#include "Utility/logger.hpp"
void MouseState::addSubscriber(MouseButton button, UserEvent event,
                               MouseObserver* subscriber) {
    if (std::find(subscriberList[button][event].begin(),
                  subscriberList[button][event].end(),
                  subscriber) == subscriberList[button][event].end()) {
        subscriberList[button][event].push_back(subscriber);
        Logger::success(
            Logger::messageAddress("Added mouse subscriber", subscriber));

    } else {
        Logger::error(Logger::messageAddress("Adding existing mouse subscriber",
                                             subscriber));
    }
}

void MouseState::removeSubscriber(MouseButton button, UserEvent event,
                                  MouseObserver* subscriber) {
    auto foundIterator =
        std::find(subscriberList[button][event].begin(),
                  subscriberList[button][event].end(), subscriber);

    if (foundIterator == subscriberList[button][event].end()) {
        Logger::error("Removing non-existent subscriber");
        return;
    }
    subscriberList[button][event].erase(foundIterator);
}

void MouseState::clearSubscriber() {
    for (auto [button, eventStorage] : subscriberList) {
        for (auto [event, observer] : eventStorage) observer.clear();
    }
}

void MouseState::clearSubscriber(MouseButton button, UserEvent event) {
    subscriberList[button][event].clear();
}

void MouseState::handleEvent(const std::optional<sf::Event>& event) {
    const auto mouseClickEvent = event->getIf<sf::Event::MouseButtonPressed>();
    const auto mouseReleaseEvent =
        event->getIf<sf::Event::MouseButtonReleased>();
    if (mouseClickEvent) {
        sf::Vector2i windowPosition = mouseClickEvent->position;
        sf::Vector2f worldPosition = window.mapPixelToCoords(windowPosition);
        if (mouseClickEvent->button == sf::Mouse::Button::Left)
            for (MouseObserver* observer :
                 subscriberList[MouseButton::Left][UserEvent::Press])
                observer->onLeftMousePress(worldPosition, windowPosition);
    
        if (mouseClickEvent->button == sf::Mouse::Button::Right)
            for (MouseObserver* observer :
                 subscriberList[MouseButton::Right][UserEvent::Press])
                observer->onRightMousePress(worldPosition, windowPosition);
    }
    if (mouseReleaseEvent) {
        sf::Vector2i windowPosition = mouseReleaseEvent->position;
        sf::Vector2f worldPosition = window.mapPixelToCoords(windowPosition);
        if (mouseReleaseEvent->button == sf::Mouse::Button::Left)
            for (MouseObserver* observer :
                 subscriberList[MouseButton::Left][UserEvent::Release])
                observer->onLeftMouseRelease(worldPosition, windowPosition);

        if (mouseReleaseEvent->button == sf::Mouse::Button::Right)
            for (MouseObserver* observer :
                 subscriberList[MouseButton::Right][UserEvent::Release])
                observer->onRightMouseRelease(worldPosition, windowPosition);
    }
}

MouseState::MouseState(sf::RenderWindow& window) : window{window} {}