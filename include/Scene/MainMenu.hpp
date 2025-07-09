/**
 * @file MainMenu.hpp
 * @brief Declares the MainMenu scene class for the main menu UI.
 *
 * The MainMenu class implements the main menu scene, providing UI buttons for navigation,
 * settings, and test actions. It manages its own UI components and integrates with the
 * scene management system.
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Core/InputManager.hpp"
#include "Core/ResourceManager.hpp"
#include "GUIComponents/button.hpp"
#include "GUIComponents/mediator.hpp"
#include "Scene/Scene.hpp"

/**
 * @class MainMenu
 * @brief Scene representing the main menu, with UI buttons and event handling.
 *
 * The MainMenu scene provides the main entry point for the game, displaying UI buttons for
 * starting the game, accessing settings, and running test actions. It handles user input,
 * updates its UI components, and renders the menu to the screen.
 */
class SceneManager;
class MainMenu : public Scene {
protected:
    Button testBtn;  ///< Test button for the main menu
    std::unique_ptr<Button> settingBtn; ///< Button to open the settings menu

    /**
     * @brief Handles test scene switching logic (for development/testing).
     */
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

    /**
     * @brief Registers UI components and event handlers for the main menu.
     */
    void registerComponents() override;

    /**
     * @brief Unregisters UI components and event handlers for the main menu.
     */
    void unRegisterComponents() override;
};