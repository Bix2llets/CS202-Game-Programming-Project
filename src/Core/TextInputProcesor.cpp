#include "Core/TextInputProcessor.hpp"

void TextInputProcessor::addSubscriber(TextInputObserver& observer) {
    observerList.push_back(&observer);
}

void TextInputProcessor::removeSubscriber(TextInputObserver& observer) {
    observerList.remove(&observer);
}

void TextInputProcessor::handleEvent(
    const std::optional<sf::Event>& event) {
    auto textEvent = event->getIf<sf::Event::TextEntered>();
    if (textEvent) {
        for (auto& observer : observerList) {
            observer->onTextInput(textEvent->unicode);
        }
    }
}