#ifndef _H_SYSTEMTIMER
#define _H_SYSTEMTIMER
#define SYSTEMTIMER_BASE 0x3000
struct SystemTimer {
    public:
        volatile unsigned int control;
        volatile unsigned int lowRegister;
        volatile unsigned int highRegister;
        volatile unsigned int compare[4];

        /// @brief Gets the current ticks since the start of the program
        /// @return The current ticks as a 64-bit value
        static unsigned long GetSystemTime();
        
        /// @brief Gets the system timer registers
        /// @return The system timer registers
        static SystemTimer* GetSystemTimer();
};
#endif