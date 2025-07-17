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
#include "GUIComponents/ButtonBuilder.hpp"
#include "GUIComponents/button.hpp"
Setting::Setting() {
    createButtons();
    setupButtonMessages();
    setupHandlers();

    setupComponentVector();
}

void Setting::onLoad() {
    for (auto &button : alwaysShownElements)
        button->subscribeMouseAll(InputManager::getInstance().getMouseState());
}

void Setting::onUnload() {
    for (auto &button : alwaysShownElements) {
        button->unSubscribeMouseAll(
            InputManager::getInstance().getMouseState());
        button->resetAnimation();
    }
}

void Setting::update() {
    for (auto &button : alwaysShownElements) button->update();
}

void Setting::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::View previousView = target.getView();
    target.setView(target.getDefaultView());
    for (auto &button : alwaysShownElements) target.draw(*button, states);
    target.setView(previousView);
}

void Setting::createButtons() {
    const sf::Vector2f buttonSize = {120, 50};
    ButtonBuilder builder(*this);
    musicVolumeDecrement = builder.reset()
                               .setPosition({220.f, 20.f})
                               .setSize(buttonSize)
                               .setNotificationMessage("Music Decrease")
                               .setText("MusicDec")
                               .loadJson("basicButton")
                               .build();

    musicVolumeIncrement = builder.reset()
                               .setPosition({370.f, 20.f})
                               .setSize(buttonSize)
                               .setNotificationMessage("Music Increase")
                               .setText("MusicInc")
                               .loadJson("basicButton")
                               .build();
    soundVolumeDecrement = builder.reset()
                               .setPosition({220.f, 220.f})
                               .setSize(buttonSize)
                               .setNotificationMessage("Sound Decrease")
                               .setText("SoundDec")
                               .loadJson("basicButton")
                               .build();

    soundVolumeIncrement = builder.reset()
                               .setPosition({370.f, 220.f})
                               .setSize(buttonSize)
                               .setNotificationMessage("Sound Increase")
                               .setText("SoundInc")
                               .loadJson("basicButton")
                               .build();

    resolution1 = builder.reset()
                      .setPosition({200.f, 400.f})
                      .setSize(buttonSize)
                      .setNotificationMessage("Resolution1")
                      .setText("Res1")
                      .loadJson("lerpTesting")
                      .build();

    resolution2 = builder.reset()
                      .setPosition({350.f, 400.f})
                      .setSize(buttonSize)
                      .setNotificationMessage("Resolution2")
                      .setText("Res2")
                      .loadJson("basicButton")
                      .build();

    resolution3 = builder.reset()
                      .setPosition({500.f, 400.f})
                      .setSize(buttonSize)
                      .setNotificationMessage("Resolution3")
                      .setText("Res3")
                      .loadJson("basicButton")
                      .build();

    backButton = builder.reset()
                     .setPosition({50.f, 50.f})
                     .setSize(buttonSize)
                     .setNotificationMessage("Main menu")
                     .setText("Main menu")
                     .loadJson("basicButton")
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
        ResourceManager::getInstance().setMusicVolume(musicVolume + 10);
        Logger::debug("Music Volume Decrease Triggered");
    });
    subscribe("Music Increase", [this](std::any sender, std::any data) {
        int musicVolume = ResourceManager::getInstance().getMusicVolume();
        ResourceManager::getInstance().setMusicVolume(musicVolume - 10);
        Logger::debug("Music Volume Increase Triggered");
    });
    subscribe("Sound Decrease", [this](std::any sender, std::any data) {
        int soundVolume = ResourceManager::getInstance().getSoundVolume();
        ResourceManager::getInstance().setSoundVolume(soundVolume - 10);
        Logger::debug("Sound Volume Decrease Triggered");
    });
    subscribe("Sound Increase", [this](std::any sender, std::any data) {
        int soundVolume = ResourceManager::getInstance().getSoundVolume();
        ResourceManager::getInstance().setSoundVolume(soundVolume - 10);
        Logger::debug("Sound Volume Increase Triggered");
    });

    subscribe("Resolution1", [this](std::any, std::any) {
        using namespace GameConstants;
        Window::getInstance().setSize(
            {DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT});
        Logger::info(std::format("Changed window size to {}x{}",
                                 DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT));
    });
    subscribe("Resolution2", [this](std::any, std::any) {
        using namespace GameConstants;
        Window::getInstance().setSize({WINDOW_WIDTH_1, WINDOW_HEIGHT_1});
        Logger::info(std::format("Changed window size to {}x{}", WINDOW_WIDTH_1,
                                 WINDOW_HEIGHT_1));
    });
    subscribe("Resolution3", [this](std::any, std::any) {
        using namespace GameConstants;
        Window::getInstance().setSize({WINDOW_WIDTH_2, WINDOW_HEIGHT_2});
        Logger::info(std::format("Changed window size to {}x{}", WINDOW_WIDTH_2,
                                 WINDOW_HEIGHT_2));
    });
    subscribe("Main menu", [this](std::any, std::any) {
        using namespace GameConstants;
        SceneManager::getInstance().changeScene("Main menu");
    });
}

void Setting::setupComponentVector() {
    alwaysShownElements.push_back(std::move(musicVolumeDecrement));
    alwaysShownElements.push_back(std::move(musicVolumeIncrement));
    alwaysShownElements.push_back(std::move(soundVolumeDecrement));
    alwaysShownElements.push_back(std::move(soundVolumeIncrement));
    alwaysShownElements.push_back(std::move(resolution1));
    alwaysShownElements.push_back(std::move(resolution2));
    alwaysShownElements.push_back(std::move(resolution3));
    alwaysShownElements.push_back(std::move(backButton));
}