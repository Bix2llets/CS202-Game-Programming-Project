#include "Scene/MainMenu.hpp"

#include "Core/InputManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/SceneManager.hpp"
#include "Core/Window.hpp"
#include "GUIComponents/RectangularButtonBuilder.hpp"
#include "Utility/Scaler.hpp"
#include "Utility/aligner.hpp"
#include "Utility/logger.hpp"
MainMenu::MainMenu() : Scene() {
    RectangularButtonBuilder builder(*this);
    playButton = builder.reset()
                     .setPosition({GameConstants::DEFAULT_WINDOW_WIDTH / 2.f -
                                       275.f / 2.f,
                                   GameConstants::DEFAULT_WINDOW_HEIGHT / 2.f})
                     .setSize({275.f, 125.f})
                     .setText("Gameplay")
                     .loadJson("borderless_background_mainmenu")
                     .setCallback([this](RectangularButton *button) {
                         Logger::debug("Turning to gameplay");
                         notify("Gameplay");
                     })
                     .setBackground(ResourceManager::getInstance().getTexture(
                         "menu_button"))
                     .setTextSize(48)
                     .build();
    settingButton =
        builder.reset()
            .setPosition(playButton->getPosition() +
                         sf::Vector2f{65.f / 2.f, 40.f} +
                         sf::Vector2f{0.f, playButton->getSize().y})
            .setSize({220.f, 100.f})
            .setText("Setting")
            .loadJson("borderless_background_mainmenu")
            .setCallback([this](RectangularButton *button) {
                Logger::debug("Setting button pressed");
                notify("Setting");
            })
            .setBackground(
                ResourceManager::getInstance().getTexture("menu_button"))
            .setTextSize(36)
            .build();

    exitButton = builder.reset()
                .setPosition(settingButton->getPosition() +
                             sf::Vector2f{0.f, settingButton->getSize().y} +
                             sf::Vector2f{22.f, 40.f})
                .setSize({176, 80})
                .setText("Exit")
                .loadJson("borderless_background_mainmenu")
                .setCallback([this](RectangularButton *button) {
                    Logger::debug("Exit button pressed");
                    notify("Exit");
                })
                .setBackground(ResourceManager::getInstance().getTexture(
                    "menu_button"))
                .setTextSize(30)
                .build();
    title = std::make_unique<sf::Sprite>(
        *ResourceManager::getInstance().getTexture("menu_title"));

    title->setScale({1.5f, 1.5f});
    title->setPosition({GameConstants::DEFAULT_WINDOW_WIDTH / 2.f,
                        title->getGlobalBounds().size.y / 2.f + 15.f});

    background = std::make_unique<sf::Sprite>(
        *ResourceManager::getInstance().getTexture("menu_background"));
    background->setScale({1.f, 1.f});
    background->setPosition({0.f, 0.f});

    Scaler::scaleSprite(*background,
                        sf::Vector2f{GameConstants::DEFAULT_WINDOW_WIDTH,
                                     GameConstants::DEFAULT_WINDOW_HEIGHT});

    Aligner::align(*title, HorizontalAlignment::Center, VerticalAlignment::Middle);
    Logger::debug("Main menu created");

    subscribe("Setting", [this](std::any, std::any) {
        SceneManager::getInstance().changeScene("Setting");
    });
    subscribe("Gameplay", [this](std::any, std::any) {
        SceneManager::getInstance().changeScene("Gameplay");
    });
}

void MainMenu::draw(sf::RenderTarget &target, sf::RenderStates state) const {
    Window::getInstance().toggleGUIMode();
    target.draw(*background, state);
    target.draw(*title, state);
    target.draw(*playButton, state);
    target.draw(*settingButton, state);
    target.draw(*exitButton, state);
}

void MainMenu::update() {
    playButton->update();
    settingButton->update();
    exitButton->update();
}

void MainMenu::testSceneSwitching() {
    Logger::debug("Scene switch initiated");
    SceneManager::getInstance().changeScene("Gameplay");
}

void MainMenu::onLoad() {
    playButton->subscribeMouseAll(InputManager::getInstance().getMouseState());
    settingButton->subscribeMouseAll(
        InputManager::getInstance().getMouseState());
    exitButton->subscribeMouseAll(
        InputManager::getInstance().getMouseState());
};

void MainMenu::onUnload() {
    playButton->unSubscribeMouseAll(
        InputManager::getInstance().getMouseState());
    settingButton->unSubscribeMouseAll(
        InputManager::getInstance().getMouseState());
    playButton->resetAnimation();
    settingButton->resetAnimation();
    exitButton->unSubscribeMouseAll(
        InputManager::getInstance().getMouseState());
    exitButton->resetAnimation();
};
