#pragma once
#include "Scene/MainMenu.hpp"

class MainMenuMock : public MainMenu {
   public:
    using MainMenu::MainMenu;
    MainMenuMock(sf::RenderWindow &window, std::string &name,
                 SceneManager &parentManager, InputManager &inputManager,
                 ResourceManager &resManager)
        : MainMenu(window, name, parentManager, inputManager, resManager) {}
    // Simulate a button click for testing
    void simulateTestButtonClick() { testBtn.click(); }
};
