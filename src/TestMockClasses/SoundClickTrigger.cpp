#include "testMockClasses/SoundClickTrigger.hpp"

#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "Utility/logger.hpp"
bool SoundClickTrigger::onLeftMousePress(const sf::Vector2f &worldPosition,
                                         const sf::Vector2f &windowPosition) {
    Logger::info("Left press");
    return true;
}

SoundClickTrigger::SoundClickTrigger() {}

bool SoundClickTrigger::onLeftMouseRelease(const sf::Vector2f &worldPosition,
                                           const sf::Vector2f &windowPosition) {
    Logger::info("Left release");
    return true;
}
bool SoundClickTrigger::onRightMousePress(const sf::Vector2f &worldPosition,
                                          const sf::Vector2f &windowPosition) {
    Logger::info("Right press");
    return true;
}
bool SoundClickTrigger::onRightMouseRelease(
    const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) {
    Logger::info("Right release");
    return true;
}

bool SoundClickTrigger::onMouseEvent(Mouse button, UserEvent event,
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
    return false;
}

bool SoundClickTrigger::onKeyEvent(Key key, UserEvent event,
                                   const sf::Vector2f &worldPosition,
                                   const sf::Vector2f &windowPosition) {
    Logger::info(std::to_string(static_cast<int>(key)));
    Logger::info(std::to_string(static_cast<int>(event)));
}

bool SoundClickTrigger::onScrollEvent(float delta,
                                      const sf::Vector2f &worldPosition,
                                      const sf::Vector2f &windowPosition) {}