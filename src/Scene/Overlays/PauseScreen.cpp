#include "Scene/Overlays/PauseScreen.hpp"

#include "Core/InputManager.hpp"
#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "Core/Window.hpp"
#include "GUIComponents/RectangularButtonBuilder.hpp"
#include "Utility/aligner.hpp"
PauseScreen::PauseScreen(Mediator& mediator) : Overlay(mediator) {
    background.setSize({500, 300});
    background = Aligner::align(background, HorizontalAlignment::Center,
                                VerticalAlignment::Middle);
    background.setFillColor(sf::Color(0xFFFFFFFF));
    // background.setOutlineColor(sf::Color(0x000000ff));
    // background.setOutlineThickness(4);

    background.setTexture(
        ResourceManager::getInstance().getTexture("pause_background"));
    darkOverlay.setSize(static_cast<sf::Vector2f>(
        Window::getInstance().getRenderWindow().getSize()));
    darkOverlay.setFillColor(sf::Color(0, 0, 0, 150));
    darkOverlay.setPosition({0, 0});
    background.setPosition(
        {Window::getInstance().getRenderWindow().getSize().x / 2.f,
         Window::getInstance().getRenderWindow().getSize().y / 2.f});
    background = Aligner::align(background, HorizontalAlignment::Center,
                                VerticalAlignment::Middle);

    title.setFont(*ResourceManager::getInstance().getFont("pixel"));
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    title.setString("Pause");
    title = Aligner::align(title, HorizontalAlignment::Center,
                           VerticalAlignment::Middle);
    title.setPosition(
        {background.getPosition().x,
         background.getPosition().y - background.getSize().y / 2});

    subscribeMouse(Mouse::Left, UserEvent::Press,
                   InputManager::getInstance().getMouseState());
    subscribeMouse(Mouse::None, UserEvent::Move,
                   InputManager::getInstance().getMouseState());
    subscribeMouse(Mouse::Left, UserEvent::Release,
                   InputManager::getInstance().getMouseState());
    constructButtons();
}

void PauseScreen::update() {
    // Update logic for the pause screen if needed
    for (std::unique_ptr<RectangularButton>& button : buttons) {
        button->update();
    }
}

void PauseScreen::render() {
    sf::RenderWindow& window = Window::getInstance().getRenderWindow();
    window.draw(darkOverlay);
    window.draw(background);
    sf::Text backText(title);
    backText.move({-5, -5});
    backText.setFillColor(sf::Color::Black);
    // window.draw(backText);
    // window.draw(title);

    // Render buttons if any
    for (const auto& button : buttons) {
        window.draw(*button);
    }
}

void PauseScreen::constructButtons() {
    RectangularButtonBuilder builder(parentScene);

    sf::Vector2f topLeftCorner = background.getGlobalBounds().position;
    sf::Vector2f size = background.getGlobalBounds().size;

    float navigationButtonY = topLeftCorner.y + size.y - 100;
    buttons.push_back(std::move(
        builder.setText("Resume")
            .setPosition({topLeftCorner.x + size.x / 2 + 10, navigationButtonY})
            .setSize({200, 50})
            .setCallback([this](RectangularButton* button) {
                parentScene.notify("resume_game");
            })
            .setBackground(
                ResourceManager::getInstance().getTexture("button_wooden"))
            .loadJson("borderless_background_basic")
            .setTextSize(30)
            .build()));

    buttons.push_back(std::move(
        builder.setText("Quit")
            .setPosition(
                {topLeftCorner.x + size.x / 2 - 10 - 200, navigationButtonY})
            .setSize({200, 50})
            .setCallback([this](RectangularButton* button) {
                parentScene.notify("quit_level");
            })
            .setBackground(
                ResourceManager::getInstance().getTexture("button_wooden"))
            .loadJson("borderless_background_basic")
            .setTextSize(30)
            .build()));
}

bool PauseScreen::onMouseEvent(Mouse mouse, UserEvent event,
                               const sf::Vector2f& worldPosition,
                               const sf::Vector2f& windowPosition) {
    bool isButtonProcessed = false;
    for (std::unique_ptr<RectangularButton>& button : buttons) {
        if (button)
            isButtonProcessed |= button->onMouseEvent(
                mouse, event, worldPosition, windowPosition);
    }
    if (isButtonProcessed) return true;
    return false;
}

PauseScreen::~PauseScreen() {
    // Clean up resources if necessary
    unSubscribeMouse(Mouse::Left, UserEvent::Press,
                     InputManager::getInstance().getMouseState());
    unSubscribeMouse(Mouse::None, UserEvent::Move,
                     InputManager::getInstance().getMouseState());
    unSubscribeMouse(Mouse::Left, UserEvent::Release,
                     InputManager::getInstance().getMouseState());

    buttons.clear();
}
