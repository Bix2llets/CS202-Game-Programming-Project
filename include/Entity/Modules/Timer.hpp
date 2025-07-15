
/**
 * @file Timer.hpp
 * @brief Defines the Timer class and TimerMode enum for timing operations.
 */

#pragma once
/**
 * @enum TimerMode
 * @brief Modes for the Timer class.
 */
enum class TimerMode {
    Single,     ///< Timer triggers once and must be reset
    Continuous  ///< Timer triggers repeatedly at intervals
};

/**
 * @enum TimerDirection
 * @brief Specifies the counting direction for the Timer.
 */
enum class TimerDirection {
    Forward,  ///< Timer counts up from zero
    Backward  ///< Timer counts down from interval
};

/**
 * @class Timer
 * @brief Utility class for managing time intervals and cooldowns.
 *
 * Supports single-use and continuous timing modes, availability checks, and
 * interval updates.
 */
class Timer {
   private:
    float timeInterval;   ///< The interval between timer triggers
    float remainingTime;  ///< Time left until the timer is available
    TimerMode mode;       ///< The mode of the timer (single or continuous)
    TimerDirection
        direction;  ///< The direction of the timer (forward or backward)
    int available;  ///< Number of available uses (for continuous mode)
    bool running; ///< Identicate whether the timer is running

   public:
    Timer()
        : timeInterval{1},
          remainingTime{1},
          mode{TimerMode::Single},
          available{0},
          direction{TimerDirection::Backward},
          running{true} {};

    /**
     * @brief Checks if the timer is available (ready to trigger).
     * @return True if available, false otherwise.
     */
    bool isAvailable();

    /**
     * @brief Resets the timer to its initial state.
     */
    Timer& reset();

    /**
     * @brief Uses the timer, reducing available count and restarting if
     * needed.
     */
    Timer& use();

    /**
     * @brief Sets a new time interval for the timer.
     * @param interval The new interval duration.
     */
    Timer& setTimeInterval(float interval);

    /**
     * @brief Updates the timer based on elapsed time.
     */
    Timer& update();

    /**
     * @brief Set the timer mode
     * @param mode
     */
    Timer& setTimerMode(TimerMode mode);
    Timer& setRemainingTime(float remaining);
    Timer& setTimerDirection(TimerDirection direction);

    inline float getCompletionPercentage() const {
        return 1 - remainingTime / timeInterval;
    }

    inline float getRemainingTime() const { return remainingTime; }
    float getPassedTime() const;

    Timer& pause();
    Timer& resume();
};