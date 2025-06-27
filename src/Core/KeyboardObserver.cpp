#include "Core/KeyboardObserver.hpp"

#include "Core/KeyboardState.hpp"
void KeyboardObserver::subscribeKeyboard(Key key, UserEvent event,
                                 KeyboardState &state) {
    state.addSubscriber(key, event, this);
}

void KeyboardObserver::unSubscribeKeyboard(Key key, UserEvent event,
                                   KeyboardState &state) {
    state.removeSubscriber(key, event, this);
}