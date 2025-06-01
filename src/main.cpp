#include <SFML/Graphics.hpp>
#include <optional>

#include "Base/Constants.hpp"
#include "Scene/SceneManager.hpp"
#include "Utility/logger.hpp"
int main() {
    sf::CircleShape circle;
    circle.setRadius(100.f);

    circle.setOrigin(sf::Vector2f{circle.getRadius(), circle.getRadius()} /
                     2.f);
    circle.setPosition({100.f, 100.f});
    circle.setFillColor(sf::Color::Blue);

    sf::RenderWindow mainWindow(sf::VideoMode({900, 900}), "TeknoMagika",
                                sf::Style::Titlebar | sf::Style::Close,
                                sf::State::Windowed);

    sf::Clock gameClock;
    float remainingTime;
    float timeElapsed;
    SceneManager manager(mainWindow);
    while (mainWindow.isOpen()) {
        // * Poll event
        sf::Time timeSinceLastFrame = gameClock.restart();
        remainingTime += timeSinceLastFrame.asSeconds();
        timeElapsed += timeSinceLastFrame.asSeconds();
        while (remainingTime > GameConstants::TICK_INTERVAL) {
            remainingTime -= GameConstants::TICK_INTERVAL;
            while (std::optional<sf::Event> event = mainWindow.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    mainWindow.close();
                    break;
                }
                auto keyPressed = event->getIf<sf::Event::KeyPressed>();
                if (keyPressed) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape)
                        mainWindow.close();
                    break;
                }
                manager.handleEvent(event);
            }
            for (int i = 0; i < GameConstants::TICK_INTERVAL /
                                    GameConstants::SUBTICK_INTERVAL;
                 i++) {
                manager.handleInput();
                manager.update();
            }
            mainWindow.display();
        }
        // }
    }
    Logger::success("Program exit success");
    // Logger::debug("Program");
}