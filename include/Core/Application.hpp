#pragma once
#include <SFML/Graphics.hpp>

#include "Scene/SceneManager.hpp"
class Application {
    private:
    sf::RenderWindow window;
    SceneManager sceneManager;

    bool isRunning;
    public:
    Application();
    void run();
    ~Application();
};