
#ifndef _H_TIME
#define _H_TIME

/// @brief A data structure representing a span of time (e.g. seconds,milliseconds)
/// @remarks Time values are based on the System Timer
struct TimeSpan {
    public:
        /// @brief The raw ticks responsible for this timespan
        unsigned long ticks;

        /// @brief Constructs a new instance of the time span given the time in ticks
        /// @param ticks The time in ticks
        TimeSpan(unsigned long ticks);

        /// @brief Gets the current time since the boot of the machine
        /// @return The current time since the boot of the machine
        static TimeSpan GetCurrentTime();
};
#endif