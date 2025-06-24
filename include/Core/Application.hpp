#pragma once
#include <SFML/Graphics.hpp>

#include "Core/SceneManager.hpp"
#include "Core/ResourceManager.hpp"
class Application {
    private:
    sf::RenderWindow window;
    SceneManager sceneManager;
    ResourceManager resourceManager;

    bool isRunning;
    public:
    Application();
    void run();
    ~Application();
};