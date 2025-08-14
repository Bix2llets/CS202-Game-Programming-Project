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
    // middlePressPosition = {0.f, 0.f};
    previousMiddleMousePosition = {0.f, 0.f};
    levelSize =
        sf::Vector2f{GameConstants::MAP_WIDTH, GameConstants::MAP_HEIGHT};
    adjustUserView();
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

bool Window::onMouseEvent(Mouse mouse, UserEvent event,
                          const sf::Vector2f& worldPosition,
                          const sf::Vector2f& windowPosition) {
    if (isLocked) return false;
    if (mouse == Mouse::Middle && event == UserEvent::Press) {
        if (isMiddlePressed == true) return false;
        isMiddlePressed = true;
        // middlePressPosition = windowPosition;  // * Can also be world
        // position, should be consistent
        previousMiddleMousePosition = windowPosition;
        // Logger::debug(std::format("Window pan Set {} {}",
        //                           userView.getCenter().x,
        //                           userView.getCenter().y));
        window.setMouseCursorGrabbed(true);
        return true;
    }

    if (mouse == Mouse::Middle && event == UserEvent::Move) {
        if (!isMiddlePressed) return false;
        sf::Vector2f displacement =
            windowPosition - previousMiddleMousePosition;
        previousMiddleMousePosition = windowPosition;
        userView.move(-displacement);
        clampView();
        window.setView(userView);
        // Logger::debug(std::format("Window pan Moving {} {}", displacement.x,
        //   displacement.y));
        return true;
    }
    if (mouse == Mouse::Middle && event == UserEvent::Release) {
        if (!isMiddlePressed) return false;
        isMiddlePressed = false;
        // middlePressPosition = {0.f, 0.f};
        previousMiddleMousePosition = {0.f, 0.f};
        // Logger::debug(std::format("Window pan Release {} {}",
        //   userView.getCenter().x,
        //   userView.getCenter().y));
        window.setMouseCursorGrabbed(false);
        return true;
    }
    return false;
}

void Window::resetView() {
    userView = window.getDefaultView();
    if (!isLocked) window.setView(userView);
}

bool Window::onScrollEvent(float delta, const sf::Vector2f& worldPosition,
                           const sf::Vector2f& windowPosition) {
    if (isLocked) return false;
    static const float ZOOM_FACTOR = 0.2f;
    Logger::debug(std::format("Scorlling {}", delta));
    userView.setSize(userView.getSize() * (1 + delta * ZOOM_FACTOR));
    userView.move((userView.getCenter() - worldPosition) * delta * ZOOM_FACTOR);
    window.setView(userView);
    clampView();
    // adjustUserView();

    InputManager::getInstance().getMouseState().updateMousePosition(
        windowPosition);
    return true;
}

void Window::adjustUserView() {
    sf::Vector2f winSize = sf::Vector2f{GameConstants::DEFAULT_WINDOW_WIDTH,
                                        GameConstants::DEFAULT_WINDOW_HEIGHT};
    float aspect = static_cast<float>(winSize.x) / winSize.y;
    float viewHeight = winSize.y;
    float viewWidth = std::min(viewHeight * aspect, winSize.x);
    viewHeight = viewWidth / aspect;
    userView.setSize({viewWidth, viewHeight});
    userView.setCenter({viewWidth / 2.f, viewHeight / 2.f});
}
void Window::clampView() {
    static const float MENU_WIDTH_PIXELS = 200.f;

    float mapRight  = levelSize.x;
    float mapBottom = levelSize.y;

    sf::Vector2f viewSize   = userView.getSize();  // world coords
    sf::Vector2f viewCenter = userView.getCenter();

    float aspect = static_cast<float>(GameConstants::DEFAULT_WINDOW_WIDTH) / GameConstants::DEFAULT_WINDOW_HEIGHT;
    float viewHeight = viewSize.y;
    float viewWidth = std::min(viewHeight * aspect, static_cast<float>(GameConstants::DEFAULT_WINDOW_WIDTH));
    viewHeight = viewWidth / aspect;

    viewSize = {viewWidth, viewHeight};
    float zoomFactor = viewSize.x / GameConstants::DEFAULT_WINDOW_WIDTH;
    float menuWorldWidth = MENU_WIDTH_PIXELS * zoomFactor;

    // Horizontal clamp
    float minX = viewSize.x / 2.f;
    float maxX = mapRight - viewSize.x / 2.f + menuWorldWidth;

    viewCenter.x = std::clamp(viewCenter.x, minX, maxX);

    // Vertical clamp
    float minY = viewSize.y / 2.f;
    float maxY = mapBottom - viewSize.y / 2.f;
    viewCenter.y = std::clamp(viewCenter.y, minY, maxY);

    userView.setCenter(viewCenter);
    userView.setSize(viewSize);
}


void Window::setLevelSize(sf::Vector2f size) {
    levelSize = size;
    clampView();
    adjustUserView();
}