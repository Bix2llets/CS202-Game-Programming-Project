#include "Scene/MainMenu.hpp"

#include "Core/InputManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/SceneManager.hpp"
#include "GUIComponents/ButtonBuilder.hpp"
#include "Utility/logger.hpp"

MainMenu::MainMenu(sf::RenderWindow &window, SceneManager &parentManager,
                   InputManager &inputManager, ResourceManager &resManager,
                   JSONLoader &loader)
    : Scene(window, parentManager, inputManager, resManager, loader),
      testBtn{"Testing", {{100.f, 100.f}, {100.f, 50.f}}, *this} {
    ButtonBuilder builder(*this, resourceManager, loader);
    settingBtn = builder.reset()
                     .setPosition({300.f, 100.f})
                     .setSize({50.f, 50.f})
                     .setText("To setting")
                     .loadJson("basicButton")
                     .setNotificationMessage("Setting")
                     .build();
    Logger::debug("Main menu created");

    testBtn.setNotificationMessage("Gameplay");
    subscribe("Setting", [this](std::any, std::any) {
        sceneManager.changeScene("Setting");
    });
    subscribe("Gameplay", [this](std::any, std::any) {
        sceneManager.changeScene("Gameplay");
    });
    testBtn.setOnClick(
        [this](Button *a) { Logger::debug("Clicked test button 1"); });
}

void MainMenu::draw(sf::RenderTarget &target, sf::RenderStates state) const {
    target.draw(testBtn, state);
    target.draw(*settingBtn, state);
}

void MainMenu::update() {}

void MainMenu::testSceneSwitching() {
    Logger::debug("Scene switch initiated");
    sceneManager.changeScene("Gameplay");
}

void MainMenu::registerComponents() {
    testBtn.subscribeMouse(Mouse::Left, UserEvent::Press,
                           inputManager.getMouseState());
    settingBtn->subscribeMouse(Mouse::Left, UserEvent::Press,
                               inputManager.getMouseState());
};

void MainMenu::unRegisterComponents() {
    testBtn.unSubscribeMouse(Mouse::Left, UserEvent::Press,
                             inputManager.getMouseState());
    settingBtn->unSubscribeMouse(Mouse::Left, UserEvent::Press,
                                 inputManager.getMouseState());
};
