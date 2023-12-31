
#ifndef _H_TIME
#define _H_TIME
#include <commons.hpp>
/// @brief A different time units that appears in the TimeSpan struct.
enum TimeUnit {
    NANOSECOND,
    MICROSECOND,
    MILLISECOND,
    SECOND
};

/// @brief A data structure representing a span of time (e.g. seconds,milliseconds)
/// @remarks Time values are based on the System Timer
struct TimeSpan {
    public:
        /// @brief The raw ticks responsible for this timespan
        ulong ticks;

        /// @brief Constructs a new instance of the time span given the time in ticks
        /// @param ticks The time in ticks
        TimeSpan(ulong ticks);

        /// @brief Constructs a new instance of the time span given the time in a
        ///        particular unit.
        /// @param amount The time amount
        /// @param unit The unit of time
        TimeSpan(ulong amount, TimeUnit unit);

        /// @brief Enters into a spin loop to wait the current timespan duration out.
        void SpinWait();

        /// @brief Adds the number of days onto this timespan
        /// @param amount The amount of days
        /// @return A new timespan resulting from the operation
        TimeSpan AddDays(ulong amount);

        /// @brief Adds the number of hours onto this timespan
        /// @param amount The amount of hours
        /// @return A new timespan resulting from the operation
        TimeSpan AddHours(ulong amount);

        /// @brief Adds the number of minutes onto this timespan
        /// @param amount The amount of minutes
        /// @return A new timespan resulting from the operation
        TimeSpan AddMinutes(ulong amount);

        /// @brief Adds the number of seconds onto this timespan
        /// @param amount The amount of seconds
        /// @return A new timespan resulting from the operation
        TimeSpan AddSeconds(ulong amount);
        
        /// @brief Adds the number of milliseconds onto this timespan
        /// @param amount The amount of milliseconds
        /// @return A new timespan resulting from the operation
        TimeSpan AddMilliseconds(ulong amount);

        /// @brief Adds the number of nanoseconds onto this timespan
        ///        (note that nanosecond tracking is inaccurate)
        /// @param amount The amount of nanoseconds
        /// @return A new timespan resulting from the operation
        TimeSpan AddNanoseconds(ulong amount);

        /// @brief Gets the current time since the boot of the machine
        /// @return The current time since the boot of the machine
        static TimeSpan GetCurrentTime();
};
#endif