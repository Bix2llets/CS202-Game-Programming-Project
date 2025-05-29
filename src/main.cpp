#include <SFML/Graphics.hpp>
#include <optional>
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

    while (mainWindow.isOpen()) {
        // * Poll event
        while (std::optional<sf::Event> event = mainWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>()) mainWindow.close();
            auto keyPressed = event->getIf<sf::Event::KeyPressed>();
            if (keyPressed) {
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                    mainWindow.close();
            }
        }





        // * Rendering
        mainWindow.clear(sf::Color::Black);
        mainWindow.draw(circle);
        mainWindow.display();
    }
}