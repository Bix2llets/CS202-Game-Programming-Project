#include "Scene/MainMenu.hpp"

#include "Core/InputManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/SceneManager.hpp"
#include "GUIComponents/ButtonBuilder.hpp"
#include "Utility/logger.hpp"

MainMenu::MainMenu(sf::RenderWindow &window, SceneManager &parentManager,
                   InputManager &inputManager, ResourceManager &resManager,
                   JSONLoader &loader)
    : Scene(window, parentManager, inputManager, resManager, loader) {
    ButtonBuilder builder(*this, resourceManager, loader);
    testBtn = builder.reset()
                  .setPosition({120.f, 100.f})
                  .setSize({120.f, 50.f})
                  .setText("Gameplay")
                  .loadJson("basicButton")
                  .setNotificationMessage("Gameplay")
                  .setCallback([this](Button *button) {
                      Logger::debug("Turning to gameplay");
                  })
                  .build();
    settingBtn = builder.reset()
                     .setPosition({300.f, 100.f})
                     .setSize({50.f, 50.f})
                     .setText("To setting")
                     .loadJson("basicButton")
                     .setNotificationMessage("Setting")
                     .setCallback([this](Button *button) {
                         Logger::debug("Setting button pressed");
                     })
                     .build();
    Logger::debug("Main menu created");

    subscribe("Setting", [this](std::any, std::any) {
        sceneManager.changeScene("Setting");
    });
    subscribe("Gameplay", [this](std::any, std::any) {
        sceneManager.changeScene("Gameplay");
    });
}

void MainMenu::draw(sf::RenderTarget &target, sf::RenderStates state) const {
    target.draw(*testBtn, state);
    target.draw(*settingBtn, state);
}

void MainMenu::update() {
    testBtn->update();
    settingBtn->update();
}

void MainMenu::testSceneSwitching() {
    Logger::debug("Scene switch initiated");
    sceneManager.changeScene("Gameplay");
}

void MainMenu::onLoad() {
    testBtn->subscribeMouseAll(inputManager.getMouseState());
    settingBtn->subscribeMouseAll(inputManager.getMouseState());
};

void MainMenu::onUnload() {
    testBtn->unSubscribeMouseAll(inputManager.getMouseState());
    settingBtn->unSubscribeMouseAll(inputManager.getMouseState());
};
