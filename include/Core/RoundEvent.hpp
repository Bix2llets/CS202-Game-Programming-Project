/**
 * @file RoundEvent.hpp
 * @brief Declares the RoundEvent enum for round-based game events.
 *
 * Used to signal the start and end of a round to game systems (e.g., towers, UI).
 */
#pragma once

/**
 * @enum RoundEvent
 * @brief Events that occur at the start or end of a round.
 */
enum class RoundEvent {
    Start, ///< The round has started
    End    ///< The round has ended
};