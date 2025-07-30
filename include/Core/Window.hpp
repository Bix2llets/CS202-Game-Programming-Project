/**
 * @brief Provides global access to a window instance using the Singleton
 * pattern, wrapping SFML's RenderWindow inside.
 *
 * This class ensures that only one instance of the window exists throughout the
 * application. It offers a global point of access to the window, allowing other
 * components to retrieve or interact with the window instance as needed.
 *
 * Note: This is different from the SFML's window, which supports multiple
 * instances and direct initialization Usage:
 * - Use getInstance() to obtain the singleton window instance.
 * - Prevents direct instantiation or copying to maintain a single global
 * instance.
 */
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "Core/MouseObserver.hpp"

class Window : public MouseObserver {
   private:
   sf::RenderWindow window;
    Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    sf::View userView;
    bool isLocked;

    bool isMiddlePressed = false;
    // sf::Vector2f middlePressPosition;
    sf::Vector2f previousMiddleMousePosition;

   public:
    static Window& getInstance();

    sf::RenderWindow& getRenderWindow();
    void resetView();

    void toggleGUIMode();
    void toggleUserMode();

    bool onMouseEvent(Mouse mouse, UserEvent event, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) override;
    bool onScrollEvent(float delta, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition);

    void adjustUserView();

    void clampView();
};