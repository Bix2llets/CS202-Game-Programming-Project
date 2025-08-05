#include "Core/TextInputObserver.hpp"

#include "Core/TextInputProcessor.hpp"
void TextInputObserver::subscribeTextInput(TextInputProcessor& processor) {
    processor.addSubscriber(*this);
}

void TextInputObserver::unsubscribeTextInput(TextInputProcessor& processor) {
    processor.removeSubscriber(*this);
}