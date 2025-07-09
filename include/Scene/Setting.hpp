#pragma once
/**
 * @file Setting.hpp
 * @brief Declares the Setting scene for game configuration options.
 *
 * The Setting class provides a scene for adjusting game settings such as music and sound volume,
 * screen resolution, and navigation back to the main menu. It manages UI button components and their
 * event handlers, and integrates with the main game loop for updates and rendering.
 */

#include "GUIComponents/button.hpp"
#include "Scene/Scene.hpp"
#include <vector>

/**
 * @class Setting
 * @brief Scene for configuring game settings (audio, resolution, etc.).
 *
 * This scene presents UI controls for adjusting music and sound volume, changing screen resolution,
 * and returning to the main menu. It manages its own set of button components and their event handlers.
 */
class Setting : public Scene {
public:
    /**
     * @brief Constructs the Setting scene.
     * @param window Reference to the main render window.
     * @param name Name of the scene.
     * @param parentManager Reference to the scene manager.
     * @param inputManager Reference to the input manager.
     * @param resManager Reference to the resource manager.
     */
    Setting(sf::RenderWindow &window, const std::string &name, 
        SceneManager &parentManager, InputManager &inputManager, ResourceManager &resManager);

    /**
     * @brief Registers UI components and event handlers for the scene.
     */
    void registerComponents() override;

    /**
     * @brief Unregisters UI components and event handlers for the scene.
     */
    void unRegisterComponents() override;

    /**
     * @brief Updates the state of the scene and its components.
     */
    void update() override;

    /**
     * @brief Draws the scene and its UI components to the target.
     * @param target The render target to draw on.
     * @param states The render states to use for drawing.
     */
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    std::unique_ptr<Button> musicVolumeIncrement;   ///< Button to increase music volume
    std::unique_ptr<Button> musicVolumeDecrement;   ///< Button to decrease music volume
    std::unique_ptr<Button> soundVolumeIncrement;   ///< Button to increase sound volume
    std::unique_ptr<Button> soundVolumeDecrement;   ///< Button to decrease sound volume

    std::unique_ptr<Button> resolution1;            ///< Button for first resolution option
    std::unique_ptr<Button> resolution2;            ///< Button for second resolution option
    std::unique_ptr<Button> resolution3;            ///< Button for third resolution option
    std::unique_ptr<Button> backButton;             ///< Button to return to the main menu

    std::vector<std::unique_ptr<Button>> alwaysShownElements; ///< Buttons always visible in the scene

    /**
     * @brief Creates and initializes all button components.
     */
    void createButtons();

    /**
     * @brief Sets up event handlers for button actions.
     */
    void setupHandlers();

    /**
     * @brief Configures button display messages.
     */
    void setupButtonMessages();

    /**
     * @brief Populates the alwaysShownElements vector with button components.
     */
    void setupComponentVector();
};