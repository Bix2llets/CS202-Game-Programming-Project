/**
 * @file Scene.hpp
 * @brief Declares the abstract Scene class, the base for all game scenes.
 */
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <optional>
#include <string>

#include "GUIComponents/Mediator.hpp"

class SceneManager;
class InputManager;
class ResourceManager;
/**
 * @class Scene
 * @brief Abstract base class for all game scenes.
 *
 * Scenes represent different states or screens in the game (e.g., menu,
 * gameplay).
 */
class Scene : public sf::Drawable, public Mediator {
   protected:
    sf::RenderWindow& window;  ///< Reference to the main window.
    std::string name;          ///< Name of the scene.
    InputManager& inputManager;
    SceneManager& sceneManager;
    ResourceManager& resourceManager;

   public:
    /**
     * @brief Constructs a Scene with the given window and name.
     * @param window Reference to the SFML render window.
     * @param name Name of the scene.
     * @param parentManager Reference to the parent SceneManager.
     * @param inputManager Reference to the InputManager singleton.
     * @param resourceManager Reference to the ResourceManager singleton.
     */
    Scene(sf::RenderWindow& window, const std::string& name,
          SceneManager& parentManager, InputManager& inputManager,
          ResourceManager& resourceManager)
        : window{window},
          name{name},
          sceneManager{parentManager},
          inputManager{inputManager},
          resourceManager{resourceManager} {};
    /**
     * @brief Gets the name of the scene.
     * @return Reference to the scene name string.
     */
    const std::string& getName() const { return name; }
    /**
     * @brief Draws the scene (pure virtual).
     * @param target The render target to draw to.
     * @param state Current render states.
     */
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates state) const = 0;
    /**
     * @brief Updates the scene (pure virtual).
     */
    virtual void update() = 0;
    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    virtual ~Scene() = default;

    /**
     * @brief Register all GUI components to the inputManager.
     */
    virtual void registerComponents() = 0;
    /**
     * @brief un-register all GUI components to the inputManager.
     */
    virtual void unRegisterComponents() = 0;

    /**
     * @brief Get font for rendering use (should be reasonable as mediator stores UI component, which need font to display text)
     * @param fontName the name of the font
     */
     const sf::Font* const getFont(std::string fontName) override;
};