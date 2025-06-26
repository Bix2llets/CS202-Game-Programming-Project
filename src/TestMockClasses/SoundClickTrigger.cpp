#include "testMockClasses/SoundClickTrigger.hpp"

#include "Core/ResourceManager.hpp"
#include "Utility/logger.hpp"
void SoundClickTrigger::onLeftMouseClick(const sf::Vector2f &worldPosition,
                                     const sf::Vector2i &windowPosition) {
    Logger::info("Left click");
}

SoundClickTrigger::SoundClickTrigger(ResourceManager &resManager)
: resManager{resManager} {}


void SoundClickTrigger::onLeftMouseRelease(const sf::Vector2f &worldPosition, 
    const sf::Vector2i &windowPosition) {

        Logger::info("Left release");
    }
void SoundClickTrigger::onRightMouseClick(const sf::Vector2f &worldPosition, 
                                    const sf::Vector2i &windowPosition) {
                                        Logger::info("Right click");
                                    }
void SoundClickTrigger::onRightMouseRelease(const sf::Vector2f &worldPosition, 
                                    const sf::Vector2i &windowPosition) {
                                        Logger::info("Right release");
                                    }