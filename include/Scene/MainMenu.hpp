/**
 * @file MainMenu.hpp
 * @brief Declares the MainMenu scene class for the main menu UI.
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

#include "Core/ResourceManager.hpp"
#include "GUIComponents/RectangularButton.hpp"
#include "GUIComponents/mediator.hpp"
#include "Scene/Scene.hpp"

/**
 * @class MainMenu
 * @brief Scene representing the main menu, with UI buttons and event handling.
 */
class MainMenu : public Scene {
    protected:
    std::unique_ptr<RectangularButton> playButton;
    std::unique_ptr<RectangularButton> settingButton;
    std::unique_ptr<RectangularButton> exitButton;
    std::unique_ptr<sf::Sprite> title;
    std::unique_ptr<sf::Sprite> background;
    void testSceneSwitching();

    public:
    MainMenu();

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

    void onLoad() override;
    void onUnload() override;
};