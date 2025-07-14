#include "Entity/Modules/Timer.hpp"

#include "Base/Constants.hpp"

bool Timer::isAvailable() { return available > 0; }

Timer& Timer::update() {
    if (mode == TimerMode::Single) {
        if (available) return *this;
        remainingTime -= GameConstants::TICK_INTERVAL;
        if (remainingTime <= 0) {
            available = 1;
            remainingTime = 0;
        }
    }

    if (mode == TimerMode::Continuous) {
        remainingTime -= GameConstants::TICK_INTERVAL;
        while (remainingTime <= 0) {
            remainingTime += timeInterval;
            available++;
        }
    }
    return *this;
}

Timer& Timer::reset() {
    available = 0;
    remainingTime = timeInterval;
    return *this;
}

Timer& Timer::use() {
    if (available <= 0) return *this;
    available--;
    if (mode == TimerMode::Single) {
        remainingTime = timeInterval;
    }
    return *this;
}

Timer& Timer::setTimeInterval(float interval) {
    timeInterval = interval;
    return *this;
}

Timer& Timer::setTimerDirection(TimerDirection direction) {
    this->direction = direction;
    return *this;
}

Timer& Timer::setTimerMode(TimerMode mode) {
    this->mode = mode;
    return *this;
}

Timer& Timer::setRemainingTime(float remaining) {
    remainingTime = remaining;
    return *this;
}