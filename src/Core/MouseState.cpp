#include "Core/MouseState.hpp"

#include <algorithm>
#include <list>
#include <sstream>
#include <utility>

#include "Base/Constants.hpp"
#include "Core/MouseObserver.hpp"
#include "Utility/SignalMap.hpp"
#include "Utility/logger.hpp"
void MouseState::addSubscriber(Mouse button, UserEvent event,
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

void MouseState::removeSubscriber(Mouse button, UserEvent event,
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

void MouseState::clearSubscriber(Mouse button, UserEvent event) {
    subscriberList[button][event].clear();
}

void MouseState::handleEvent(const std::optional<sf::Event>& event) {
    using namespace GameConstants;
    const auto mouseClickEvent = event->getIf<sf::Event::MouseButtonPressed>();
    const auto mouseReleaseEvent =
        event->getIf<sf::Event::MouseButtonReleased>();
    if (mouseClickEvent) {
        sf::Vector2f windowPosition =
            static_cast<sf::Vector2f>(mouseClickEvent->position);
        sf::Vector2f worldPosition =
            window.mapPixelToCoords(mouseClickEvent->position);
        windowPosition = scalePosition(windowPosition);
        worldPosition = scalePosition(worldPosition);
        Logger::info(std::format("{} {} {} {}", windowPosition.x,
                                 windowPosition.y, worldPosition.x,
                                 worldPosition.y));

        Mouse pressedButton =
            SignalMap::mapSfmlMouseButton(mouseClickEvent->button);
        for (MouseObserver* observer :
             subscriberList[pressedButton][UserEvent::Press])
            observer->onMouseEvent(pressedButton, UserEvent::Press,
                                   worldPosition, windowPosition);
        return;
    }
    if (mouseReleaseEvent) {
        sf::Vector2f windowPosition =
            static_cast<sf::Vector2f>(mouseReleaseEvent->position);
        sf::Vector2f worldPosition =
            window.mapPixelToCoords(mouseReleaseEvent->position);
        windowPosition = scalePosition(windowPosition);
        worldPosition = scalePosition(worldPosition);
        Logger::info(std::format("{} {} {} {}", windowPosition.x,
                                 windowPosition.y, worldPosition.x,
                                 worldPosition.y));

        Mouse pressedButton =
            SignalMap::mapSfmlMouseButton(mouseReleaseEvent->button);
        for (MouseObserver* observer :
             subscriberList[pressedButton][UserEvent::Release])
            observer->onMouseEvent(pressedButton, UserEvent::Release,
                                   worldPosition, windowPosition);
        return;
    }
}

MouseState::MouseState(sf::RenderWindow& window) : window{window} {}

sf::Vector2f MouseState::scalePosition(sf::Vector2f input) {
    input.x = input.x / window.getSize().x * GameConstants::DEFAULT_WINDOW_WIDTH;
    input.y = input.y / window.getSize().y * GameConstants::DEFAULT_WINDOW_HEIGHT;
    return input;
}