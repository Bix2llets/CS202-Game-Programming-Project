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

    constructButtons();
}

void PauseScreen::update() {
    // Update logic for the pause screen if needed
    for (std::unique_ptr<RectangularButton>& button : buttons) {
        button->update();
    }
}

void PauseScreen::render() const {
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
    float controlButtonY = topLeftCorner.y + size.y - 230;
    buttons.push_back(std::move(
        builder.reset()
            .setText("Resume")
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
        builder.reset()
            .setText("Quit")
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
    buttons.push_back(std::move(
        builder.reset()
            .setPosition({topLeftCorner.x + size.x - 60, topLeftCorner.y + 20})
            .setSize({40, 40})
            .setBackground(
                ResourceManager::getInstance().getTexture("close_button"))
            .setCallback([this](RectangularButton* button) {
                parentScene.notify("resume_game");
            })
            .loadJson("borderless_background_basic")
            .build()));

    buttons.push_back(std::move(
        builder.reset()
            .setSize({120, 120})
            .setPosition(
                {topLeftCorner.x + size.x / 2 - 150 - 20, controlButtonY})
            .setBackground(
                ResourceManager::getInstance().getTexture("sound_normal"))
            .loadJson("borderless_background_basic")
            .setCallback([this](RectangularButton* button) {
                parentScene.notify("toggle_sound");
                if (ResourceManager::getInstance().isSoundEnabled()) {
                    button->setBackground(
                        ResourceManager::getInstance().getTexture(
                            "sound_normal"));
                } else {
                    button->setBackground(
                        ResourceManager::getInstance().getTexture(
                            "sound_mute"));
                }
            })
            .build()));

    buttons.push_back(std::move(
        builder.reset()
            .setSize({120, 120})
            .setPosition(
                {topLeftCorner.x + size.x / 2 + 20 + 15 + 15, controlButtonY})
            .setBackground(
                ResourceManager::getInstance().getTexture("music_normal"))
            .loadJson("borderless_background_basic")
            .setCallback([this](RectangularButton* button) {
                parentScene.notify("toggle_music");
                if (ResourceManager::getInstance().isMusicEnabled()) {
                    button->setBackground(
                        ResourceManager::getInstance().getTexture(
                            "music_normal"));
                } else {
                    button->setBackground(
                        ResourceManager::getInstance().getTexture(
                            "music_mute"));
                }
            })
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

    buttons.clear();
}
