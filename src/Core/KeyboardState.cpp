#include "Core/KeyboardState.hpp"

#include <algorithm>
#include <list>
#include <map>
#include <optional>

<<<<<<< HEAD
#include "Core/KeyboardObserver.hpp"
#include "Core/UserEvent.hpp"
#include "Utility/logger.hpp"
#include "Utility/SignalMap.hpp"
=======
#include "Base/Constants.hpp"
#include "Core/KeyboardObserver.hpp"
#include "Core/UserEvent.hpp"
#include "Utility/SignalMap.hpp"
#include "Utility/logger.hpp"
>>>>>>> Entity
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
    sf::Vector2f windowPosition =
        static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    sf::Vector2f worldPosititon =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));
    windowPosition = scalePosition(windowPosition);
    worldPosititon = scalePosition(worldPosititon);
    if (keyPress) {
        Key key = SignalMap::mapSfmlKey(keyPress->code);
        std::list<KeyboardObserver*> observerList =
            subscriberList[key][UserEvent::Press];
        for (auto subscriber : observerList) {
            subscriber->onKeyEvent(key, UserEvent::Press, worldPosititon,
                                   windowPosition);
        }
        return;
    }
    if (keyRelease) {
        Key key = SignalMap::mapSfmlKey(keyRelease->code);
        std::list<KeyboardObserver*> observerList =
            subscriberList[key][UserEvent::Release];
        for (auto subscriber : observerList) {
            subscriber->onKeyEvent(key, UserEvent::Release, worldPosititon,
                                   windowPosition);
        }
        return;
    }
}

KeyboardState::KeyboardState(sf::RenderWindow& window) : window{window} {}
sf::Vector2f KeyboardState::scalePosition(sf::Vector2f input) {
    input.x =
        input.x / window.getSize().x * GameConstants::DEFAULT_WINDOW_WIDTH;
    input.y =
        input.y / window.getSize().y * GameConstants::DEFAULT_WINDOW_HEIGHT;
    return input;
}
