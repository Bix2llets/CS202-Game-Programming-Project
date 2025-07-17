#include "Core/Window.hpp"

#include "Base/Constants.hpp"

sf::RenderWindow& Window::getInstance() {
    static sf::RenderWindow instance(
        sf::VideoMode({GameConstants::DEFAULT_WINDOW_WIDTH,
                       GameConstants::DEFAULT_WINDOW_HEIGHT}),
        "Rampart Remain", sf::Style::Close | sf::Style::Titlebar);
    return instance;
}