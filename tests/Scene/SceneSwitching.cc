#include <gtest/gtest.h>

#include "Core/InputManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/SceneManager.hpp"
#include "MainMenuMock.hpp"
#include "Scene/BlankScene.hpp"
TEST(sceneSwitching, actualSwitching) {
    sf::RenderWindow window;
    InputManager inputManager(window);
    SceneManager sceneManager(window);
    ResourceManager resourceManager;
    resourceManager.loadFont(
        "assets/fonts/League_Spartan/static/LeagueSpartan-Medium.ttf",
        "LeagueSpartan");

    MainMenuMock mainMenu(window, "Main Menu", sceneManager, inputManager,
                          resourceManager);
    sceneManager.registerScene<BlankScene>("Gameplay", inputManager,
                                           resourceManager);
    mainMenu.simulateTestButtonClick();
    EXPECT_STREQ(sceneManager.getCurrentScene()->getName().c_str(), "Gameplay");
}
TEST(sceneSwitching, nullSwitching) {
    sf::RenderWindow window;
    InputManager inputManager(window);
    SceneManager sceneManager(window);
    ResourceManager resourceManager;
    resourceManager.loadFont(
        "assets/fonts/League_Spartan/static/LeagueSpartan-Medium.ttf",
        "LeagueSpartan");

    MainMenuMock mainMenu(window, "Main Menu", sceneManager, inputManager,
                          resourceManager);
    // sceneManager.registerScene<BlankScene>("Gameplay", inputManager,
    // resourceManager);
    mainMenu.simulateTestButtonClick();
    EXPECT_EQ(sceneManager.getCurrentScene(), nullptr);
}