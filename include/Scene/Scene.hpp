/**
 * @file Scene.hpp
 * @brief Declares the abstract Scene class, the base for all game scenes.
 */
#pragma once
#include <SFML/Graphics.hpp>
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
    InputManager& inputManager;
    SceneManager& sceneManager;
    ResourceManager& resourceManager;

   public:
    /**
     * @brief Constructs a Scene with the given window and name.
     * @param window Reference to the SFML render window.
     * @param parentManager Reference to the parent SceneManager.
     * @param inputManager Reference to the InputManager .
     * @param resourceManager Reference to the ResourceManager .
     */
    Scene(SceneManager& parentManager,
          InputManager& inputManager, ResourceManager& resourceManager)
        :           sceneManager{parentManager},
          inputManager{inputManager},
          resourceManager{resourceManager} {};
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
    virtual void onLoad() = 0;
    /**
     * @brief un-register all GUI components to the inputManager.
     */
    virtual void onUnload() = 0;

    [[deprecated("Prefer builder/factory for fonts loading")]]
    /**
     * @brief Get font for rendering use (should be reasonable as mediator
     * stores UI component, which need font to display text)
     * @param fontName the name of the font
     */
    const sf::Font* const getFont(std::string fontName) override;

    /**
     * @brief Load a texture into the ResourceManager
     * @param path Path to the texture file
     * @param ID Unique identifier for the texture
     */
    void loadTexture(const std::string& path, const std::string& ID);

    /**
     * @brief Get a texture from the ResourceManager
     * @param ID Unique identifier for the texture
     * @return Pointer to the texture, or nullptr if not found
     */
    const sf::Texture* const getTexture(const std::string& ID);
};