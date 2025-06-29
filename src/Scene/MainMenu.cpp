#include "Scene/MainMenu.hpp"

#include "Core/InputManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/SceneManager.hpp"
#include "Utility/logger.hpp"

MainMenu::MainMenu(sf::RenderWindow &window, const std::string &name,
                   SceneManager &parentManager, InputManager &inputManager,
                   ResourceManager &resManager)
    : Scene(window, name, parentManager, inputManager, resManager),
      testBtn{"Testing", {{100.f, 100.f}, {100.f, 50.f}}, testGroup} {
    Logger::debug("Main menu created");
    testBtn.setOnClick([](Button *a) { Logger::debug("Clicked test button"); });
    testBtn.subscribeMouse(Mouse::Left, UserEvent::Press,
                           inputManager.getMouseState());
}

void MainMenu::draw(sf::RenderTarget &target, sf::RenderStates state) const {
    target.draw(testBtn);
}

void MainMenu::update() {}