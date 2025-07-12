#include "Scene/Setting.hpp"

#include <any>
#include <format>

#include "Base/Constants.hpp"
#include "Core/InputManager.hpp"
#include "Core/MouseObserver.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "GUIComponents/button.hpp"

#include "Core/SceneManager.hpp"
Setting::Setting(sf::RenderWindow &window,
                 SceneManager &parentManager, InputManager &inputManager,
                 ResourceManager &resManager)
    : Scene(window, parentManager, inputManager, resManager) {
    createButtons();
    setupButtonMessages();
    setupHandlers();

    setupComponentVector();
}

void Setting::registerComponents() {
    for (auto &button : alwaysShownElements)
        button->subscribeMouse(Mouse::Left, UserEvent::Press,
                               inputManager.getMouseState());
}

void Setting::unRegisterComponents() {
    for (auto &button : alwaysShownElements)
        button->unSubscribeMouse(Mouse::Left, UserEvent::Press,
                                 inputManager.getMouseState());
}

void Setting::update() {}

void Setting::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::View previousView = target.getView();
    target.setView(target.getDefaultView());
    for (auto &button : alwaysShownElements) target.draw(*button, states);
    target.setView(previousView);
}

void Setting::createButtons() {
    sf::Vector2f buttonSize = {120, 50};
    musicVolumeDecrement = std::make_unique<Button>(
        "musicDec", sf::FloatRect({220.0, 20.0}, buttonSize), *this);
    musicVolumeIncrement = std::make_unique<Button>(
        "musicInc", sf::FloatRect({370.0, 20.0}, buttonSize), *this);
    soundVolumeDecrement = std::make_unique<Button>(
        "volumeDec", sf::FloatRect({220.0, 220.0}, buttonSize), *this);
    soundVolumeIncrement = std::make_unique<Button>(
        "volumeInc", sf::FloatRect({370.0, 220.0}, buttonSize), *this);

    resolution1 = std::make_unique<Button>(
        "Res1", sf::FloatRect{{200, 400}, buttonSize}, *this);
    resolution2 = std::make_unique<Button>(
        "Res1", sf::FloatRect{{350, 400}, buttonSize}, *this);
    resolution3 = std::make_unique<Button>(
        "Res1", sf::FloatRect{{500, 400}, buttonSize}, *this);

    backButton = std::make_unique<Button>(
        "Main menu", sf::FloatRect{{50, 50}, buttonSize}, *this);
}

void Setting::setupButtonMessages() {
    musicVolumeDecrement->setNotificationMessage("Music Decrease");
    musicVolumeIncrement->setNotificationMessage("Music Increase");
    soundVolumeDecrement->setNotificationMessage("Sound Decrease");
    soundVolumeIncrement->setNotificationMessage("Sound Increase");
    resolution1->setNotificationMessage("Resolution1");
    resolution2->setNotificationMessage("Resolution2");
    resolution3->setNotificationMessage("Resolution3");
    backButton->setNotificationMessage("Main menu");

}

void Setting::setupHandlers() {
    subscribe("Music Decrease", [this](std::any sender, std::any data) {
        int musicVolume = this->resourceManager.getMusicVolume();
        resourceManager.setMusicVolume(musicVolume + 10);
        Logger::debug("Music Volume Decrease Triggered");
    });
    subscribe("Music Increase", [this](std::any sender, std::any data) {
        int musicVolume = this->resourceManager.getMusicVolume();
        resourceManager.setMusicVolume(musicVolume - 10);
        Logger::debug("Music Volume Increase Triggered");
    });
    subscribe("Sound Decrease", [this](std::any sender, std::any data) {
        int soundVolume = this->resourceManager.getSoundVolume();
        resourceManager.setSoundVolume(soundVolume - 10);
        Logger::debug("Sound Volume Decrease Triggered");
    });
    subscribe("Sound Increase", [this](std::any sender, std::any data) {
        int soundVolume = this->resourceManager.getSoundVolume();
        resourceManager.setSoundVolume(soundVolume - 10);
        Logger::debug("Sound Volume Increase Triggered");
    });

    subscribe("Resolution1", [this](std::any, std::any) {
        using namespace GameConstants;
        window.setSize({DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT});
        Logger::info(std::format("Changed window size to {}x{}",
                                 DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT));
    });
    subscribe("Resolution2", [this](std::any, std::any) {
        using namespace GameConstants;
        window.setSize({WINDOW_WIDTH_1, WINDOW_HEIGHT_1});
        Logger::info(std::format("Changed window size to {}x{}", WINDOW_WIDTH_1,
                                 WINDOW_HEIGHT_1));
    });
    subscribe("Resolution3", [this](std::any, std::any) {
        using namespace GameConstants;
        window.setSize({WINDOW_WIDTH_2, WINDOW_HEIGHT_2});
        Logger::info(std::format("Changed window size to {}x{}", WINDOW_WIDTH_2,
                                 WINDOW_HEIGHT_2));
    });
    subscribe("Main menu", [this](std::any, std::any) {
        using namespace GameConstants;
        sceneManager.changeScene("Main menu");
    
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