#include "Core/MouseObserver.hpp"

#include "Core/MouseState.hpp"
void MouseObserver::subscribe(MouseButton button, MouseEvent event,
                              MouseState &mouseState) {
    mouseState.addSubscriber(button, event, this);
}

void MouseObserver::unSubscribe(MouseButton button, MouseEvent event, 
                                MouseState &mouseState) {
    mouseState.removeSubscriber(button, event, this);
}
