#include "testMockClasses/SoundClickTrigger.hpp"

#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "Utility/logger.hpp"
void SoundClickTrigger::onLeftMousePress(const sf::Vector2f &worldPosition,
                                         const sf::Vector2f &windowPosition) {
    Logger::info("Left press");
}

SoundClickTrigger::SoundClickTrigger(ResourceManager &resManager)
    : resManager{resManager} {}

void SoundClickTrigger::onLeftMouseRelease(const sf::Vector2f &worldPosition,
                                           const sf::Vector2f &windowPosition) {
    Logger::info("Left release");
}
void SoundClickTrigger::onRightMousePress(const sf::Vector2f &worldPosition,
                                          const sf::Vector2f &windowPosition) {
    Logger::info("Right press");
}
void SoundClickTrigger::onRightMouseRelease(
    const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) {
    Logger::info("Right release");
}

void SoundClickTrigger::onMouseEvent(Mouse button, UserEvent event,
                                     const sf::Vector2f &worldPosition,
                                     const sf::Vector2f &windowPosition) {
    if (button == Mouse::Left) {
        if (event == UserEvent::Press)
            return onLeftMousePress(worldPosition, windowPosition);
        if (event == UserEvent::Release)
            return onLeftMouseRelease(worldPosition, windowPosition);
    }

    if (button == Mouse::Right) {
        if (event == UserEvent::Press)
            return onRightMousePress(worldPosition, windowPosition);
        if (event == UserEvent::Release)
            return onRightMouseRelease(worldPosition, windowPosition);
    }
}

void SoundClickTrigger::onKeyEvent(Key key, UserEvent event,
                                   const sf::Vector2f &worldPosition,
                                   const sf::Vector2f &windowPosition) {
    Logger::info(std::to_string(static_cast<int>(key)));
    Logger::info(std::to_string(static_cast<int>(event)));
}