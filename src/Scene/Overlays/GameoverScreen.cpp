#include "Scene/Overlays/GameoverScreen.hpp"

#include "Base/Constants.hpp"
#include "Core/InputManager.hpp"
#include "Core/MouseState.hpp"
#include "Core/Window.hpp"
#include "GUIComponents/RectangularButtonBuilder.hpp"
#include "Utility/aligner.hpp"
#include "Utility/logger.hpp"
void GameoverScreen::constructButtons() {
    RectangularButtonBuilder builder(*this);
    auto restartButton =
        builder.reset()
            .setSize({150.f, 50.f})
            .setPosition({background.getPosition().x + 20.f,
                          background.getPosition().y +
                              background.getSize().y / 2 - 50.f - 35.f})
            .setText("Restart")
            .loadJson("borderless_background_basic")
            .setCallback([this](RectangularButton *button) {
                Logger::debug("Restart button pressed");
                parentScene.notify("restart_level");
            })
            .setBackground(
                ResourceManager::getInstance().getTexture("button_wooden"))
            .setTextSize(24)
            .build();

    auto exitButton =
        builder.reset()
            .setPosition({background.getPosition().x - 150.f - 20.f,
                          background.getPosition().y +
                              background.getSize().y / 2 - 50.f - 35.f})
            .setSize({150.f, 50.f})
            .setText("Exit")
            .loadJson("borderless_background_basic")
            .setCallback([this](RectangularButton *button) {
                Logger::debug("Exit button pressed");
                parentScene.notify("quit_level");
            })
            .setBackground(
                ResourceManager::getInstance().getTexture("button_wooden"))
            .setTextSize(24)
            .build();
    buttons.push_back(std::move(exitButton));
    buttons.push_back(std::move(restartButton));
}

GameoverScreen::GameoverScreen(Mediator &mediator)
    : Overlay(mediator),
      message1{*ResourceManager::getInstance().getFont("pixel")},
      message2{*ResourceManager::getInstance().getFont("pixel")} {
    background.setFillColor(sf::Color(0xFFFFFFFF));
    background.setSize({500, 300});
    darkOverlay.setFillColor(sf::Color(0, 0, 0, 150));
    darkOverlay.setSize({GameConstants::DEFAULT_WINDOW_WIDTH,
                         GameConstants::DEFAULT_WINDOW_HEIGHT});
    title.setString("Game Over");
    title.setCharacterSize(48);
    title.setFillColor(sf::Color::White);
    title.setFont(*ResourceManager::getInstance().getFont("pixel"));
    title.setPosition(
        {background.getPosition().x + background.getSize().x / 2.f,
         background.getPosition().y});
    Aligner::align(background, HorizontalAlignment::Center,
                   VerticalAlignment::Middle);
    Aligner::align(title, HorizontalAlignment::Center,
                   VerticalAlignment::Middle);

    background.setPosition({GameConstants::DEFAULT_WINDOW_WIDTH / 2.f,
                            GameConstants::DEFAULT_WINDOW_HEIGHT / 2.f});

    background.setTexture(
        ResourceManager::getInstance().getTexture("pause_background"));

    message1.setString("The zombie has overtaken");
    message1.setFillColor(sf::Color(0xE3D8BCFF));
    message1.setCharacterSize(40);
    Aligner::align(message1, HorizontalAlignment::Center,
                   VerticalAlignment::Middle);
    message1.setPosition(background.getPosition() - sf::Vector2f(0, 60.f));
    message2.setString("your defense");
    message2.setFillColor(sf::Color(0xE3D8BCFF));
    message2.setCharacterSize(40);
    Aligner::align(message2, HorizontalAlignment::Center,
                   VerticalAlignment::Middle);
    message2.setPosition(
        message1.getPosition() +
        sf::Vector2f(0, message1.getGlobalBounds().size.y + 20.f));
    title.setString("You lose!");
    title.setFillColor(sf::Color(0xBD4C14FF));
    title.setCharacterSize(90);
    title.setOutlineColor(sf::Color(0x39270FFF));
    title.setOutlineThickness(2);
    Aligner::align(title, HorizontalAlignment::Center,
                   VerticalAlignment::Middle);
    title.setPosition(background.getPosition() -
                      sf::Vector2f{0, background.getSize().y} / 2.f);
    constructButtons();
}

GameoverScreen::~GameoverScreen() { buttons.clear(); }

bool GameoverScreen::onMouseEvent(Mouse mouse, UserEvent event,
                                  const sf::Vector2f &worldPosition,
                                  const sf::Vector2f &windowPosition) {
    bool handled = false;
    for (int i = 0; i < buttons.size(); i++) {
        handled |= buttons[i]->onMouseEvent(mouse, event, worldPosition,

                                            windowPosition);
    }
    return handled;
}

void GameoverScreen::update() {
    for (const auto &button : buttons) {
        button->update();
    }
}

void GameoverScreen::render() const {
    sf::RenderWindow &window = Window::getInstance().getRenderWindow();
    window.draw(darkOverlay, sf::RenderStates::Default);
    window.draw(background, sf::RenderStates::Default);
    for (const auto &button : buttons) {
        window.draw(*button, sf::RenderStates::Default);
    }
    window.draw(title);
    window.draw(message1);
    window.draw(message2);
}