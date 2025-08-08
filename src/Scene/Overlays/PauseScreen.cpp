#include "Scene/Overlays/PauseScreen.hpp"

#include "Core/ResourceManager.hpp"
#include "Core/Window.hpp"
#include "Utility/aligner.hpp"
PauseScreen::PauseScreen() {
    background.setSize({300, 500});
    background = Aligner::align(background, HorizontalAlignment::Center,
                                VerticalAlignment::Middle);
    background.setFillColor(sf::Color(0x293131ff));
    background.setOutlineColor(sf::Color(0x000000ff));
    background.setOutlineThickness(4);
    darkOverlay.setSize(static_cast<sf::Vector2f>(
        Window::getInstance().getRenderWindow().getSize()));
    darkOverlay.setFillColor(sf::Color(0, 0, 0, 150));
    darkOverlay.setPosition({0, 0});
    title.setFont(*ResourceManager::getInstance().getFont("pixel"));
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    title.setString("Pause");
    title = Aligner::align(title, HorizontalAlignment::Center,
                           VerticalAlignment::Middle);
    title.setPosition(
        {background.getPosition().x,
         background.getPosition().y - background.getSize().y / 2});
}

void PauseScreen::update() {
    // Update logic for the pause screen if needed
}

void PauseScreen::render() {
    sf::RenderWindow& window = Window::getInstance().getRenderWindow();
    window.draw(darkOverlay);
    window.draw(background);
    window.draw(title);

    // Render buttons if any
    for (const auto& button : buttons) {
        window.draw(button);
    }
}