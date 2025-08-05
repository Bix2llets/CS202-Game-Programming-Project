/**
 * @file InputManager.hpp
 * @brief Declares the InputManager class for handling input events.
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

#include "Core/KeyboardState.hpp"
#include "Core/MouseState.hpp"
#include "Core/TextInputProcessor.hpp"
/**
 * @class InputManager
 * @brief Handles input events and manages mouse state.
 */
class InputManager {
    MouseState mouseState;  
    KeyboardState keyboardState;
    TextInputProcessor textInputProcessor;  
   public:
   
   /**
    * @brief Handles an input event.
    * @param event Optional SFML event to handle.
     */
    void handleEvent(std::optional<sf::Event>& event);
    /**
     * @brief Returns a reference to the MouseState.
     * @return Reference to the MouseState object.
     */
    inline MouseState& getMouseState() { return mouseState; };
    inline KeyboardState& getKeyboardState() { return keyboardState; }
    inline TextInputProcessor& getTextInputProcessor() { return textInputProcessor; }
    private:
    InputManager();
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    public:
    static InputManager& getInstance();
};