#include "Scene/MainMenu.hpp"

#include "Utility/logger.hpp"

#include "Core/ResourceManager.hpp"
#include "Core/InputManager.hpp"

MainMenu::MainMenu(sf::RenderWindow &window, const std::string &name,
                    InputManager &inputManager, ResourceManager &resManager)
    : Scene(window, name),
      testBtn{"Testing", {{100.f, 100.f}, {100.f, 50.f}}, testGroup} {
    Logger::debug("Main menu created");
    testBtn.setOnClick([](Button* a) {
        Logger::debug("Clicked test button");
    });
    testBtn.subscribeMouse(Mouse::Left, UserEvent::Press, inputManager.getMouseState());
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    target.draw(testBtn);
}

void MainMenu::update() {}