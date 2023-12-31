#ifndef _H_SYSTEMTIMER
#define _H_SYSTEMTIMER
#include <commons.hpp>
#define SYSTEMTIMER_BASE 0x3000
#define SYSTEMTIMER_FREQ 1000000 // BCM System Timer
struct SystemTimer {
    public:
        volatile uint control;
        volatile uint lowRegister;
        volatile uint highRegister;
        volatile uint compare[4];

        /// @brief Gets the current ticks since the start of the program
        /// @return The current ticks as a 64-bit value
        static ulong GetSystemTime();
        
        /// @brief Gets the system timer registers
        /// @return The system timer registers
        static SystemTimer* GetSystemTimer();

        /// @brief Gets the system timer frequency
        /// @return The system timer frequency
        static uint GetSystemTimerFrequency();
};
#endif