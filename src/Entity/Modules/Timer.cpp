#include "Entity/Modules/Timer.hpp"
#include "Base/Constants.hpp"
Timer::Timer(float timeInterval, TimerMode mode)
    : timeInterval{timeInterval}, remainingTime{0}, mode{mode}, available{0} {}
Timer::Timer(float timeInterval, float remainingTime, TimerMode mode)
    : timeInterval{timeInterval}, remainingTime{remainingTime}, mode{mode}, available{0} {}

bool Timer::isAvailable() {
    return available > 0;
}

void Timer::update() {
    if (mode == TimerMode::Single) {
        if (available) return;
        remainingTime -= GameConstants::TICK_INTERVAL;
        if (remainingTime <= 0) {
            available = 1;
            remainingTime = 0;
        }
        return;
    }

    if (mode == TimerMode::Continuous) {
        remainingTime -= GameConstants::TICK_INTERVAL;
        while(remainingTime <= 0) {
            remainingTime += timeInterval;
            available++;
        }
    }
}

void Timer::reset() {
    available = 0;
    remainingTime = timeInterval;
}

void Timer::use() {
    if (available <= 0) return;
    available --;
    if (mode == TimerMode::Single) {
        remainingTime = timeInterval;
    }
}

void Timer::setTimeInterval(float interval) {
    timeInterval = interval;
}