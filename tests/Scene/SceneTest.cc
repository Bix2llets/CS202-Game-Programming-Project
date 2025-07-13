#include <gtest/gtest.h>

#include "Core/SceneManager.hpp"
#include "Scene/BlankScene.hpp"

#include <SFML/Graphics.hpp>

#include "Core/InputManager.hpp"
#include "Core/ResourceManager.hpp"
TEST(nullptrTest, null) {
    sf::RenderWindow window;
    SceneManager sceneManager(window);
    auto currScene = sceneManager.getCurrentScene();
    EXPECT_EQ(currScene, nullptr);
}

TEST(nameTest, 1) {
    sf::RenderWindow window;
    SceneManager sceneManager(window);
    InputManager inputManager(window);
    JSONLoader loader;
    ResourceManager resManager;
    sceneManager.registerScene<BlankScene>("Scene1", inputManager, resManager, loader);
    sceneManager.changeScene("Scene1");
    auto currScene = sceneManager.getCurrentScene();
    EXPECT_NE(currScene, nullptr);

    // EXPECT_STREQ(sceneManager.getCurrentScene()->getName().c_str(), "Scene1");
}
TEST(nameTest, 2) {
    sf::RenderWindow window;
    SceneManager sceneManager(window);
    InputManager inputManager(window);
    ResourceManager resManager;
    JSONLoader loader;
    sceneManager.registerScene<BlankScene>("Scene1", inputManager, resManager, loader);
    sceneManager.registerScene<BlankScene>("Scene2", inputManager, resManager, loader);
    sceneManager.changeScene("Scene1");
    sceneManager.changeScene("Scene2");
    auto currScene = sceneManager.getCurrentScene();
    EXPECT_NE(currScene, nullptr);

    // EXPECT_STREQ(sceneManager.getCurrentScene()->getName().c_str(), "Scene2");
}