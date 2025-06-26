/**
 * @file UserEvent.hpp
 * @brief Defines the UserEvent enum class for representing user input events.
 *
 * The UserEvent enum is used to distinguish between different types of user-generated events
 * such as mouse or keyboard actions (e.g., Release, Press). This enum can be shared by multiple
 * input handling classes to provide a unified way to represent user actions.
 */

#pragma once

/**
 * @enum UserEvent
 * @brief Enum representing user input events (e.g., Release, Press).
 */
enum class UserEvent { Release, Press };


