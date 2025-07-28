#include "Core/Window.hpp"

#include "Base/Constants.hpp"
#include "Core/InputManager.hpp"
#include "Core/MouseState.hpp"
#include "Core/UserEvent.hpp"
Window::Window()
    : window(sf::VideoMode({GameConstants::DEFAULT_WINDOW_WIDTH,
                            GameConstants::DEFAULT_WINDOW_HEIGHT}),
             "Rampart Remain", sf::Style::Close | sf::Style::Titlebar) {
    subscribeMouse(Mouse::Middle, UserEvent::Press,
                   InputManager::getInstance().getMouseState());
    subscribeMouse(Mouse::Middle, UserEvent::Move,
                   InputManager::getInstance().getMouseState());
    subscribeMouse(Mouse::Middle, UserEvent::Release,
                   InputManager::getInstance().getMouseState());
    subscribeMouse(Mouse::Scroll, UserEvent::None,
                   InputManager::getInstance().getMouseState());
    isMiddlePressed = false;
    middlePressPosition = {0.f, 0.f};
    previousMiddleMousePosition = {0.f, 0.f};
}
Window& Window::getInstance() {
    static Window instance;
    return instance;
}

sf::RenderWindow& Window::getRenderWindow() { return window; }
void Window::toggleGUIMode() {
    window.setView(window.getDefaultView());
    isLocked = true;
}

void Window::toggleUserMode() {
    window.setView(userView);
    isLocked = false;
}

void Window::onMouseEvent(Mouse mouse, UserEvent event,
                          const sf::Vector2f& worldPosition,
                          const sf::Vector2f& windowPosition) {
    if (isLocked) return;
    if (mouse == Mouse::Middle && event == UserEvent::Press) {
        if (isMiddlePressed == true) return;
        isMiddlePressed = true;
        middlePressPosition = windowPosition;  // * Can also be world position,
                                               // should be consistent
        previousMiddleMousePosition = windowPosition;
        Logger::debug(std::format("Window pan Set {} {}",
                                  userView.getCenter().x,
                                  userView.getCenter().y));
        window.setMouseCursorGrabbed(true);
        return;
    }

    if (mouse == Mouse::Middle && event == UserEvent::Move) {
        if (!isMiddlePressed) return;
        sf::Vector2f displacement =
            windowPosition - previousMiddleMousePosition;
        previousMiddleMousePosition = windowPosition;
        userView.move(-displacement);
        window.setView(userView);
        Logger::debug(std::format("Window pan Moving {} {}", displacement.x,
                                  displacement.y));
    }
    if (mouse == Mouse::Middle && event == UserEvent::Release) {
        if (!isMiddlePressed) return;
        isMiddlePressed = false;
        middlePressPosition = {0.f, 0.f};
        previousMiddleMousePosition = {0.f, 0.f};
        Logger::debug(std::format("Window pan Release {} {}",
                                  userView.getCenter().x,
                                  userView.getCenter().y));
        window.setMouseCursorGrabbed(false);
    }
}

void Window::resetView() {
    userView = window.getDefaultView();
    if (!isLocked) window.setView(userView);
}

void Window::onScrollEvent(float delta, const sf::Vector2f& worldPosition,
                           const sf::Vector2f& windowPosition) {
    if (isLocked) return;
    static const float ZOOM_FACTOR = 0.2f;
    Logger::debug(std::format("Scorlling {}", delta));
    userView.setSize(userView.getSize() * (1 + delta * ZOOM_FACTOR));
    userView.move((userView.getCenter() - worldPosition) * delta * ZOOM_FACTOR);
    window.setView(userView);
}