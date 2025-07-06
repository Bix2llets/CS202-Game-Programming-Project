#pragma once

class Cooldown {
    float interval;
    float remaining;

   public:
    void reduceCooldown(float time) {
        remaining -= time;
        if (remaining < 0) remaining = 0;
    }

    inline bool isReady() { return remaining == 0; }
    inline void resetTimer() { remaining = 0; }
    inline void setInterval(float value) { interval = value; }
    inline float getInterval() const { return interval; }
    inline float getRemaining() const { return remaining; }
};