#ifndef APP_SYSTEMINFO_H
#define APP_SYSTEMINFO_H
#include <app.hpp>

class SystemInfoApp : public App {
    public:
        /// @brief Creates a new system info app.
        SystemInfoApp();
        
        /// @brief Initialises the app at the start of the OS.
        void _Init();

        /// @brief Updates the app with the given core.
        ///        Use Refresh at end of update to ensure that screen is refreshed (core 0)
        /// @param core The core number that is being updated with (0..3)
        void _UpdateCore(int core);

        /// @brief Renders the app on-screen
        /// @param gfx The graphics to render with
        void _Render(Graphics gfx);
};

#endif