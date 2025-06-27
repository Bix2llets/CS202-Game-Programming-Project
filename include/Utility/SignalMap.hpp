
/**
 * @file signalMap.hpp
 * @brief Provides mapping functions between SFML input enums and user-defined enums.
 *
 * This file declares the SignalMap utility class, which contains static methods to convert
 * SFML mouse and keyboard button enums to user-defined Mouse and Key enums for unified input handling.
 */
#pragma once
#include <SFML/Graphics.hpp>

/**
 * @enum Mouse
 * @brief User-defined enum for mouse buttons (definition not shown here).
 */
enum class Mouse;
/**
 * @enum Key
 * @brief User-defined enum for keyboard keys (definition not shown here).
 */
enum class Key;

/**
 * @class SignalMap
 * @brief Utility class for mapping SFML input enums to user-defined enums.
 *
 * SignalMap provides static methods to convert SFML mouse and keyboard button enums
 * (sf::Mouse::Button and sf::Keyboard::Key) to the project's own Mouse and Key enums.
 */
class SignalMap {
    public:
        /**
         * @brief Maps an SFML mouse button to a user-defined Mouse enum value.
         * @param button The SFML mouse button to map.
         * @return The corresponding Mouse enum value.
         */
        static Mouse mapSfmlMouseButton(sf::Mouse::Button button);
        /**
         * @brief Maps an SFML keyboard key to a user-defined Key enum value.
         * @param key The SFML keyboard key to map.
         * @return The corresponding Key enum value.
         */
        static Key mapSfmlKey(sf::Keyboard::Key key);
};