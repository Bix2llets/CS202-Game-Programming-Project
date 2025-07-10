
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
    int available;        ///< Number of available uses (for continuous mode)

   public:
   Timer() = default;
    /**
     * @brief Constructs a Timer with a given interval and mode.
     * @param timeInterval The interval duration.
     * @param mode The timer mode (single or continuous).
     */
    Timer(float timeInterval, TimerMode mode);

    /**
     * @brief Constructs a Timer with a given interval, remaining time, and
     * mode.
     * @param timeInterval The interval duration.
     * @param remainingTime The initial remaining time.
     * @param mode The timer mode.
     */
    Timer(float timeInterval, float remainingTime, TimerMode mode);

    /**
     * @brief Checks if the timer is available (ready to trigger).
     * @return True if available, false otherwise.
     */
    bool isAvailable();

    /**
     * @brief Resets the timer to its initial state.
     */
    void reset();

    /**
     * @brief Uses the timer, reducing available count and restarting if
     * needed.
     */
    void use();

    /**
     * @brief Sets a new time interval for the timer.
     * @param interval The new interval duration.
     */
    void setTimeInterval(float interval);

    /**
     * @brief Updates the timer based on elapsed time.
     */
    void update();
};