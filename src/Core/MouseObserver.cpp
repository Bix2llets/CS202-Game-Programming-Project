#include "Core/MouseObserver.hpp"

#include "Core/MouseState.hpp"
void MouseObserver::subscribe(MouseButton button, UserEvent event,
                              MouseState &mouseState) {
    mouseState.addSubscriber(button, event, this);
}

void MouseObserver::unSubscribe(MouseButton button, UserEvent event, 
                                MouseState &mouseState) {
    mouseState.removeSubscriber(button, event, this);
}
