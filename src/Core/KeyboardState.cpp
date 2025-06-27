#include "Core/KeyboardState.hpp"

#include <algorithm>
#include <list>
#include <map>
#include <optional>

#include "Core/KeyboardObserver.hpp"
#include "Core/UserEvent.hpp"
#include "Utility/logger.hpp"
#include "Utility/SignalMap.hpp"
void KeyboardState::addSubscriber(Key key, UserEvent event,
                                  KeyboardObserver* subscriber) {
    std::list<KeyboardObserver*>& subscribers = subscriberList[key][event];
    if (std::find(subscribers.begin(), subscribers.end(), subscriber) ==
        subscribers.end()) {
        subscribers.push_back(subscriber);
        return;
    }
    Logger::error("Inserting existed subscriber");
    return;
}

void KeyboardState::removeSubscriber(Key key, UserEvent event,
                                     KeyboardObserver* subscriber) {
    auto& subscribers = subscriberList[key][event];
    auto place = std::find(subscribers.begin(), subscribers.end(), subscriber);
    if (place == subscribers.end()) {
        Logger::error("Unsubscribing non-exist subscriber");
        return;
    }
    subscribers.erase(place);
}

void KeyboardState::clearSubscriber(Key key, UserEvent event) {
    auto& subscribers = subscriberList[key][event];
    subscribers.clear();
}

void KeyboardState::clearSubscriber() {
    for (auto [key, eventMap] : subscriberList)
        for (auto [event, subscribers] : eventMap) subscribers.clear();
}

void KeyboardState::handleEvent(std::optional<sf::Event>& event) {
    auto keyPress = event->getIf<sf::Event::KeyPressed>();
    auto keyRelease = event->getIf<sf::Event::KeyReleased>();
    auto windowPosition = sf::Mouse::getPosition(window);
    auto worldPosititon = window.mapPixelToCoords(windowPosition);
    if (keyPress) {
        Key key = SignalMap::mapSfmlKey(keyPress->code);
        for (auto subscriber : subscriberList[key][UserEvent::Press]) {
            subscriber->onKeyEvent(key, UserEvent::Press, worldPosititon,
                                   windowPosition);
        }
        return;
    }
    if (keyRelease) {
        Key key = SignalMap::mapSfmlKey(keyRelease->code);
        for (auto subscriber : subscriberList[key][UserEvent::Release]) {
            subscriber->onKeyEvent(key, UserEvent::Release, worldPosititon,
                                   windowPosition);
        }
        return;
    }
}

KeyboardState::KeyboardState(sf::RenderWindow& window) : window{window} {}