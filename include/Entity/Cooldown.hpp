#pragma once

class Cooldown {
    float interval;
    float remaining;

public:
    // Constructor to set initial interval
    explicit Cooldown(float interval = 0.0f) : interval(interval), remaining(0.0f) {}

    void reduceCooldown(float time) {
        remaining -= time;
        if (remaining < 0) remaining = 0;
    }

    // Start the cooldown timer
    void start() { remaining = interval; }
    
    // Check if cooldown is complete and ready to use
    inline bool isReady() const { return remaining <= 0; }
    
    // Reset the timer (make it ready immediately)
    inline void resetTimer() { remaining = 0; }
    
    // Set new interval and optionally restart
    void setInterval(float value, bool restart = false) { 
        interval = value; 
        if (restart) start();
    }
    
    inline float getInterval() const { return interval; }
    inline float getRemaining() const { return remaining; }
    
    // Get progress as percentage (0.0 = just started, 1.0 = ready)
    float getProgress() const { 
        return interval > 0 ? 1.0f - (remaining / interval) : 1.0f; 
    }
};