#include <hardware/systemtimer.hpp>
#include <system.hpp>

SystemTimer* SystemTimer::GetSystemTimer() {
    return (SystemTimer*) (PERIPHERAL_BASE + SYSTEMTIMER_BASE);
}

unsigned int SystemTimer::GetSystemTimerFrequency() {
    return SYSTEMTIMER_FREQ;
}

unsigned long SystemTimer::GetSystemTime() {
    SystemTimer* timer = SystemTimer::GetSystemTimer();

    // An error (resulting in an incorrect) 
    // might occur when reading the rapidly changing time.
    // - If the low value is about to overflow, then the high value will change
    //   after the low value is already set, then the low value is incorrect.
    // - If the low value doesn't overflow when read, and the high value changes
    //   after the low value overflows, then both values is incorrect.
    unsigned long lowValue = timer->lowRegister;
    unsigned long highValue = timer->highRegister;

    // To avoid the issue, just ensure if the high value did change then both values
    // get updated.

    if (timer->highRegister != highValue) {
        lowValue = timer->lowRegister;
        highValue = timer->highRegister;
    }

    return (highValue << 32) | lowValue;
};