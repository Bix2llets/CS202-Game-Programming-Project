/**
 * @file MainMenu.hpp
 * @brief Declares the MainMenu scene class for the main menu UI.
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "Core/InputManager.hpp"
#include "Core/ResourceManager.hpp"
#include "GUIComponents/button.hpp"
#include "GUIComponents/mediator.hpp"
#include "Scene/Scene.hpp"

#include <memory>

/**
 * @class MainMenu
 * @brief Scene representing the main menu, with UI buttons and event handling.
 */
class SceneManager;
class MainMenu : public Scene {
   protected:
    Button testBtn;  ///< Test button for the main menu
    Mediator testGroup;  ///< Mediator for handling button group events
    std::unique_ptr<Button> settingBtn;
    void testSceneSwitching();
   public:
    /**
     * @brief Constructs the MainMenu scene.
     * @param window Reference to the SFML render window.
     * @param name Name of the scene.
     * @param parentManager Reference to the parent SceneManager.
     * @param inputManager Reference to the InputManager singleton.
     * @param resManager Reference to the ResourceManager singleton.
     */
    MainMenu(sf::RenderWindow &window, const std::string &name, SceneManager& parentManager,
             InputManager &inputManager, ResourceManager &resManager);

    /**
     * @brief Updates the main menu scene (handles logic, input, etc).
     */
    void update();

    /**
     * @brief Draws the main menu scene and its UI components.
     * @param target The render target to draw to.
     * @param state Current render states.
     */
    void draw(sf::RenderTarget &target, sf::RenderStates state) const;

    void registerComponents() override;
    void unRegisterComponents() override;
};