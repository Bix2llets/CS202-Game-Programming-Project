#include "Scene/Setting.hpp"

#include <any>
#include <format>

#include "Base/Constants.hpp"
#include "Core/InputManager.hpp"
#include "Core/MouseObserver.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/SceneManager.hpp"
#include "Core/UserEvent.hpp"
#include "Core/Window.hpp"
#include "GUIComponents/RectangularButton.hpp"
#include "GUIComponents/RectangularButtonBuilder.hpp"
#include "Utility/aligner.hpp"

Setting::Setting() : musicVolumeDisplay(*ResourceManager::getInstance().getFont("text")),
                     soundVolumeDisplay(*ResourceManager::getInstance().getFont("text")) {
    createButtons();
    setupButtonMessages();
    setupHandlers();

    setupComponentVector();

    musicVolumeDisplay.setCharacterSize(24);
    soundVolumeDisplay.setCharacterSize(24);
    musicVolumeDisplay.setFillColor(sf::Color::White);
    musicVolumeDisplay.setOutlineColor(sf::Color::Black);
    musicVolumeDisplay.setOutlineThickness(2);
    soundVolumeDisplay.setFillColor(sf::Color::White);
    soundVolumeDisplay.setOutlineColor(sf::Color::Black);
    soundVolumeDisplay.setOutlineThickness(2);
}

void Setting::onLoad() {
    for (auto& button : alwaysShownElements)
        button->subscribeMouseAll(InputManager::getInstance().getMouseState());
}

void Setting::onUnload() {
    for (auto& button : alwaysShownElements) {
        button->unSubscribeMouseAll(
            InputManager::getInstance().getMouseState());
        button->resetAnimation();
    }
}

void Setting::update() {
    for (auto& button : alwaysShownElements) button->update();
    soundVolumeDisplay.setString(
        std::format("Sound Volume: {}", ResourceManager::getInstance().getSoundVolume()));

    musicVolumeDisplay.setString(
        std::format("Music Volume: {}", ResourceManager::getInstance().getMusicVolume()));

    Aligner::align(musicVolumeDisplay);
    Aligner::align(soundVolumeDisplay); 

    musicVolumeDisplay.setPosition({
        (musicVolumeDecrement->getPosition().x + musicVolumeDecrement->getSize().x + musicVolumeIncrement->getPosition().x) / 2.f,
        (musicVolumeDecrement->getPosition().y + musicVolumeDecrement->getSize().y / 2.f)
    });
    soundVolumeDisplay.setPosition({
        (soundVolumeDecrement->getPosition().x + soundVolumeDecrement->getSize().x + soundVolumeIncrement->getPosition().x) / 2.f,
        (soundVolumeDecrement->getPosition().y + soundVolumeDecrement->getSize().y / 2.f)
    });
    resolveQueue();
}

void Setting::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    Window::getInstance().toggleGUIMode();
    for (auto& button : alwaysShownElements) target.draw(*button, states);
    target.draw(musicVolumeDisplay, states);
    target.draw(soundVolumeDisplay, states);
}

void Setting::createButtons() {
    const sf::Vector2f buttonSize = {60, 60};
    RectangularButtonBuilder builder(*this);
    musicVolumeDecrement = builder.reset()
                               .setPosition({100.f, 170.f})
                               .setSize(buttonSize)
                               .setCallback([this](RectangularButton* button) {
                                   notify("Music Decrease");
                               })
                               .loadJson("borderless_background_basic")
                               .setBackground(
                                   ResourceManager::getInstance().getTexture(
                                       "minus_button"))
                               .build();

    musicVolumeIncrement = builder.reset()
                               .setPosition({600.f, 170.f})
                               .setSize(buttonSize)
                               .setCallback([this](RectangularButton* button) {
                                   notify("Music Increase");
                               })
                               .loadJson("borderless_background_basic")
                               .setBackground(
                                   ResourceManager::getInstance().getTexture(
                                       "plus_button"))
                               .build();
    soundVolumeDecrement = builder.reset()
                               .setPosition({100.f, 370.f})
                               .setSize(buttonSize)
                               .setCallback([this](RectangularButton* button) {
                                   notify("Sound Decrease");
                               })
                               .loadJson("borderless_background_basic")
                               .setBackground(
                                   ResourceManager::getInstance().getTexture(
                                       "minus_button"))
                               .build();

    soundVolumeIncrement = builder.reset()
                               .setPosition({600.f, 370.f})
                               .setSize(buttonSize)
                               .setCallback([this](RectangularButton* button) {
                                   notify("Sound Increase");
                               })
                               .loadJson("borderless_background_basic")
                               .setBackground(
                                   ResourceManager::getInstance().getTexture(
                                       "plus_button"))
                               .build();


    backButton = builder.reset()
                     .setPosition({100.f, 50.f})
                     .setSize({132.f, 60.f})
                     .setCallback([this](RectangularButton* button) {
                         notify("Main menu");
                     })
                     .loadJson("borderless_background_basic")
                     .setBackground(
                         ResourceManager::getInstance().getTexture("back_button"))
                     .build();
}

void Setting::setupButtonMessages() {
    // musicVolumeDecrement->setNotificationMessage("Music Decrease");
    // musicVolumeIncrement->setNotificationMessage("Music Increase");
    // soundVolumeDecrement->setNotificationMessage("Sound Decrease");
    // soundVolumeIncrement->setNotificationMessage("Sound Increase");
    // resolution1->setNotificationMessage("Resolution1");
    // resolution2->setNotificationMessage("Resolution2");
    // resolution3->setNotificationMessage("Resolution3");
    // backButton->setNotificationMessage("Main menu");
}

void Setting::setupHandlers() {
    subscribe("Music Decrease", [this](std::any sender, std::any data) {
        int musicVolume = ResourceManager::getInstance().getMusicVolume();
        ResourceManager::getInstance().setMusicVolume(musicVolume - 10);
        Logger::debug("Music Volume Decrease Triggered");
    });
    subscribe("Music Increase", [this](std::any sender, std::any data) {
        int musicVolume = ResourceManager::getInstance().getMusicVolume();
        ResourceManager::getInstance().setMusicVolume(musicVolume + 10);
        Logger::debug("Music Volume Increase Triggered");
    });
    subscribe("Sound Decrease", [this](std::any sender, std::any data) {
        int soundVolume = ResourceManager::getInstance().getSoundVolume();
        ResourceManager::getInstance().setSoundVolume(soundVolume - 10);
        Logger::debug("Sound Volume Decrease Triggered");
    });
    subscribe("Sound Increase", [this](std::any sender, std::any data) {
        int soundVolume = ResourceManager::getInstance().getSoundVolume();
        ResourceManager::getInstance().setSoundVolume(soundVolume + 10);
        Logger::debug("Sound Volume Increase Triggered");
    });

    subscribe("Resolution1", [this](std::any, std::any) {
        using namespace GameConstants;
        Window::getInstance().getRenderWindow().setSize(
            {DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT});
        Window::getInstance().adjustUserView();
        Logger::info(std::format("Changed window size to {}x{}",
                                 DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT));
    });
    subscribe("Resolution2", [this](std::any, std::any) {
        using namespace GameConstants;
        Window::getInstance().getRenderWindow().setSize(
            {WINDOW_WIDTH_1, WINDOW_HEIGHT_1});
        Logger::info(std::format("Changed window size to {}x{}", WINDOW_WIDTH_1,
                                 WINDOW_HEIGHT_1));
    });
    subscribe("Resolution3", [this](std::any, std::any) {
        using namespace GameConstants;
        Window::getInstance().getRenderWindow().setSize(
            {WINDOW_WIDTH_2, WINDOW_HEIGHT_2});
        Window::getInstance().adjustUserView();
        Logger::info(std::format("Changed window size to {}x{}", WINDOW_WIDTH_2,
                                 WINDOW_HEIGHT_2));
    });
    subscribe("Main menu", [this](std::any, std::any) {
        using namespace GameConstants;
        SceneManager::getInstance().enqueueSceneChange("Main menu");
    });
}

void Setting::setupComponentVector() {
    alwaysShownElements.push_back(musicVolumeDecrement.get());
    alwaysShownElements.push_back(musicVolumeIncrement.get());
    alwaysShownElements.push_back(soundVolumeDecrement.get());
    alwaysShownElements.push_back(soundVolumeIncrement.get());
    alwaysShownElements.push_back(backButton.get());
}