#include <data/time.hpp>
#include <hardware/systemtimer.hpp>

#pragma region TimeSpan
TimeSpan::TimeSpan(unsigned long ticks) {
    this->ticks = ticks;
}

TimeSpan TimeSpan::GetCurrentTime() {
    return TimeSpan(SystemTimer::GetSystemTime());
}

#pragma endregion