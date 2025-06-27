#include "Core/MouseObserver.hpp"

#include "Core/MouseState.hpp"
void MouseObserver::subscribeMouse(Mouse button, UserEvent event,
                              MouseState &mouseState) {
    mouseState.addSubscriber(button, event, this);
}

void MouseObserver::unSubscribeMouse(Mouse button, UserEvent event, 
                                MouseState &mouseState) {
    mouseState.removeSubscriber(button, event, this);
}
