#include "Core/Window.hpp"

#include "Base/Constants.hpp"
std::unique_ptr<sf::RenderWindow> Window::instance;

sf::RenderWindow* Window::getInstance() {
    if (instance == nullptr)
        instance = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(
            sf::VideoMode({GameConstants::DEFAULT_WINDOW_WIDTH,
                           GameConstants::DEFAULT_WINDOW_HEIGHT}),
            "Rampart Remain", sf::Style::Close | sf::Style::Titlebar));
    return instance.get();
}