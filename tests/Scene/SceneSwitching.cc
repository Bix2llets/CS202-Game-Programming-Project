#include <gtest/gtest.h>

#include "Core/InputManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/SceneManager.hpp"
#include "MainMenuMock.hpp"
#include "Scene/BlankScene.hpp"
TEST(sceneSwitching, actualSwitching) {
    sf::RenderWindow window;
    InputManager inputManager();
    SceneManager sceneManager(window);
    ResourceManager resourceManager;
    JSONLoader loader;
    resourceManager.loadFont(
        "assets/fonts/League_Spartan/static/LeagueSpartan-Medium.ttf",
        "LeagueSpartan");

    MainMenuMock mainMenu(sceneManager, inputManager,
                          resourceManager, loader);
    sceneManager.registerScene<BlankScene>("Gameplay", inputManager,
                                           resourceManager, loader);
    mainMenu.simulateTestButtonClick();
}
TEST(sceneSwitching, nullSwitching) {
    sf::RenderWindow window;
    InputManager inputManager();
    SceneManager sceneManager(window);
    ResourceManager resourceManager;
    JSONLoader loader;
    resourceManager.loadFont(
        "assets/fonts/League_Spartan/static/LeagueSpartan-Medium.ttf",
        "LeagueSpartan");

    MainMenuMock mainMenu(window, sceneManager, inputManager,
                          resourceManager, loader);
    // sceneManager.registerScene<BlankScene>("Gameplay", inputManager,
    // resourceManager);
    mainMenu.simulateTestButtonClick();
    EXPECT_EQ(sceneManager.getCurrentScene(), nullptr);
}