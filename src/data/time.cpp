#include <data/time.hpp>
#include <hardware/systemtimer.hpp>

#pragma region TimeSpan
TimeSpan::TimeSpan(unsigned long ticks) {
    this->ticks = ticks;
}

TimeSpan::TimeSpan(unsigned long amount, TimeUnit unit) {
    unsigned int timerFrequency = SystemTimer::GetSystemTimerFrequency();
    // Ticks per nanoseconds = timerFrequency per second / 1000000
    switch(unit) {
        case TimeUnit::NANOSECOND:
            this->ticks = amount * timerFrequency / 1000000000;
            break;
        case TimeUnit::MICROSECOND:
            this->ticks = amount * timerFrequency / 1000000;
            break;
        case TimeUnit::MILLISECOND:
            this->ticks = amount * timerFrequency / 1000;
            break;
        case TimeUnit::SECOND:
            this->ticks = amount * timerFrequency;
            break;
    }
}

TimeSpan TimeSpan::GetCurrentTime() {
    return TimeSpan(SystemTimer::GetSystemTime());
}

void TimeSpan::SpinWait() {
    unsigned long currentTime = SystemTimer::GetSystemTime();
    unsigned long targetTime = currentTime + this->ticks;
    while (currentTime < targetTime) {
        currentTime = SystemTimer::GetSystemTime();
    }
}
#pragma endregion