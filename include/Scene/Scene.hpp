#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <optional>
// * Base for the state machine of scenes
// * Abstract classes, not to instantize
// * Also singleton
class Scene : public sf::Drawable {
   protected:
    sf::RenderWindow &window;
    std::string name;
   public:
    Scene(sf::RenderWindow &window, const std::string &name) : window{window}, name{name} {};
    const std::string& getName() const {return name;}
    virtual void handleEvent(std::optional<sf::Event>& event) = 0;
    virtual void handleInput() = 0;
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates state) const = 0;
    virtual void update() = 0;
    virtual ~Scene() = default;
};