#pragma once
#include "Scene/Scene.hpp"


// * Only for unit testing
class BlankScene : public Scene {
    public:
    BlankScene(sf::RenderWindow &window, const std::string &name) : Scene{window, name} {}
    void handleEvent(std::optional<sf::Event> & event) {}
    void handleInput() {}
    void draw(sf::RenderTarget &target, sf::RenderStates state) const {}
    void update() {}
    ~BlankScene() = default;
};