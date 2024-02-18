#include <data/time.hpp>
#include <hardware/systemtimer.hpp>
#include <commons.hpp>
#pragma region TimeSpan
TimeSpan::TimeSpan(ulong ticks) {
    this->ticks = ticks;
}

TimeSpan::TimeSpan(ulong amount, TimeUnit unit) {
    uint timerFrequency = SystemTimer::GetSystemTimerFrequency();
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

TimeSpan TimeSpan::AddNanoseconds(ulong amount) {
    return TimeSpan(this->ticks + TimeSpan(amount, ::NANOSECOND).ticks);
}

TimeSpan TimeSpan::AddMilliseconds(ulong amount) {
    return TimeSpan(this->ticks + TimeSpan(amount, ::MILLISECOND).ticks);
}

int TimeSpan::GetSecondComponent() {
    long ticksInMinute = TimeSpan(60, ::SECOND).ticks;
    long ticksInSecond = TimeSpan(1, ::SECOND).ticks;
    return (this->ticks % ticksInMinute) / ticksInSecond;
}

TimeSpan TimeSpan::AddSeconds(ulong amount) {
    return TimeSpan(this->ticks + TimeSpan(amount, ::SECOND).ticks);
}

int TimeSpan::GetMinuteComponent() {
    long ticksInHour = TimeSpan(60*60, ::SECOND).ticks;
    long ticksInMinute = TimeSpan(60, ::SECOND).ticks;
    return (this->ticks % ticksInHour) / ticksInMinute;
}

TimeSpan TimeSpan::AddMinutes(ulong amount) {
    return this->AddSeconds(amount * 60);
}

int TimeSpan::GetHourComponent() {
    long ticksInDay = TimeSpan(60*60*24, ::SECOND).ticks;
    long ticksInHour = TimeSpan(60*60, ::SECOND).ticks;
    return (this->ticks % ticksInDay) / ticksInHour;
}

TimeSpan TimeSpan::AddHours(ulong amount) {
    return this->AddMinutes(amount * 60);
}

TimeSpan TimeSpan::AddDays(ulong amount) {
    return this->AddHours(amount * 24);
}

TimeSpan TimeSpan::GetCurrentTime() {
    return TimeSpan(SystemTimer::GetSystemTime());
}

void TimeSpan::SpinWait() {
    ulong currentTime = SystemTimer::GetSystemTime();
    ulong targetTime = currentTime + this->ticks;
    while (currentTime < targetTime) {
        currentTime = SystemTimer::GetSystemTime();
    }
}
#pragma endregion