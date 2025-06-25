/**
 * @file Scene.hpp
 * @brief Declares the abstract Scene class, the base for all game scenes.
 */
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <optional>
/**
 * @class Scene
 * @brief Abstract base class for all game scenes.
 *
 * Scenes represent different states or screens in the game (e.g., menu, gameplay).
 */
class Scene : public sf::Drawable {
   protected:
    sf::RenderWindow &window; ///< Reference to the main window.
    std::string name; ///< Name of the scene.
   public:
    /**
     * @brief Constructs a Scene with the given window and name.
     * @param window Reference to the SFML render window.
     * @param name Name of the scene.
     */
    Scene(sf::RenderWindow &window, const std::string &name) : window{window}, name{name} {};
    /**
     * @brief Gets the name of the scene.
     * @return Reference to the scene name string.
     */
    const std::string& getName() const {return name;}
    /**
     * @brief Handles an input event.
     * @param event Optional SFML event to handle.
     */
    virtual void handleEvent(std::optional<sf::Event>& event) = 0;
    /**
     * @brief Handles real-time input.
     */
    virtual void handleInput() = 0;
    /**
     * @brief Draws the scene to the given render target.
     * @param target The render target.
     * @param state The render states.
     */
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates state) const = 0;
    /**
     * @brief Updates the scene.
     */
    virtual void update() = 0;
    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    virtual ~Scene() = default;
};