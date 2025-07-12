#include "Scene/MainMenu.hpp"

#include "Core/InputManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/SceneManager.hpp"
#include "Utility/logger.hpp"

MainMenu::MainMenu(sf::RenderWindow &window, SceneManager &parentManager,
                   InputManager &inputManager, ResourceManager &resManager)
    : Scene(window, parentManager, inputManager, resManager),
      testBtn{"Testing", {{100.f, 100.f}, {100.f, 50.f}}, *this} {
    Logger::debug("Main menu created");
    settingBtn = std::make_unique<Button>(
        "To setting", sf::FloatRect{{300.f, 100.f}, {50.f, 50.f}}, *this);
    settingBtn->setNotificationMessage("Setting");

    subscribe("Setting", [this](std::any, std::any) {
        sceneManager.changeScene("Setting");
    });
    testBtn.setOnClick([this](Button *a) {
        Logger::debug("Clicked test button 1");
        this->testSceneSwitching();
    });
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
