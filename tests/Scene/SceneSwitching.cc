#include <gtest/gtest.h>

#include "MainMenuMock.hpp"
#include "Core/InputManager.hpp"
#include "Core/SceneManager.hpp"
#include "Core/ResourceManager.hpp"

#include "Scene/BlankScene.hpp"
TEST(sceneSwitching, actualSwitching) {
    sf::RenderWindow window;
    InputManager inputManager(window);
    SceneManager sceneManager(window);
    ResourceManager resourceManager;
    MainMenuMock mainMenu(window, "Main Menu", sceneManager, inputManager, resourceManager);
    sceneManager.registerScene<BlankScene>("Gameplay", inputManager, resourceManager);
    mainMenu.simulateTestButtonClick();
    EXPECT_STREQ(sceneManager.getCurrentScene()->getName().c_str(), "Gameplay");
}
TEST(sceneSwitching, nullSwitching) {
    sf::RenderWindow window;
    InputManager inputManager(window);
    SceneManager sceneManager(window);
    ResourceManager resourceManager;
    MainMenuMock mainMenu(window, "Main Menu", sceneManager, inputManager, resourceManager);
    // sceneManager.registerScene<BlankScene>("Gameplay", inputManager, resourceManager);
    mainMenu.simulateTestButtonClick();
    EXPECT_EQ(sceneManager.getCurrentScene(), nullptr);
}