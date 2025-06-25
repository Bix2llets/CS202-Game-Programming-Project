#include "testMockClasses/SoundClickTrigger.hpp"

#include "Core/ResourceManager.hpp"

void SoundClickTrigger::onLeftMouseClick(const sf::Vector2f &worldPosition,
                                     const sf::Vector2i &windowPosition) {
    resManager.playSound("coin");
}

SoundClickTrigger::SoundClickTrigger(ResourceManager &resManager)
    : resManager{resManager} {}


void SoundClickTrigger::onLeftMouseRelease(const sf::Vector2f &worldPosition, 
                                    const sf::Vector2i &windowPosition) {}
void SoundClickTrigger::onRightMouseClick(const sf::Vector2f &worldPosition, 
                                    const sf::Vector2i &windowPosition) {}
void SoundClickTrigger::onRightMouseRelease(const sf::Vector2f &worldPosition, 
                                    const sf::Vector2i &windowPosition) {}