/**
 * @brief Provides global access to a window instance using the Singleton pattern, wrapping SFML's RenderWindow inside.
 *
 * This class ensures that only one instance of the window exists throughout the application.
 * It offers a global point of access to the window, allowing other components to retrieve
 * or interact with the window instance as needed.
 *
 * Note: This is different from the SFML's window, which supports multiple instances and direct initialization
 * Usage:
 * - Use getInstance() to obtain the singleton window instance.
 * - Prevents direct instantiation or copying to maintain a single global instance.
 */
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>


class Window {
    private:

    Window(); 
    Window(const Window&) = delete; 
    Window& operator=(const Window&) = delete; 

    public:
    static sf::RenderWindow& getInstance();
};