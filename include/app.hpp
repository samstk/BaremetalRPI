#ifndef APP_H
#define APP_H
#include <data/string.hpp>
#include <gfx/gfx.hpp>

/// @brief An app that can run on this simple OS.
class App {
    public:
        /// @brief Constructs an app.
        App();

        /// @brief The name of the app
        String name;

        /// @brief The preview image of the app in BMP format (256 x 256).
        char* image = NULL;

        /// @brief If true, then the screen must be refreshed and the app rendered.
        bool refreshScreen = false;

        typedef void (*InitFunction)(App*); 
        /// @brief Initialises the app at the start of the OS.
        InitFunction Init = NULL;

        typedef void (*UpdateFunction)(App*, int);
        /// @brief Updates the app with the given core.
        ///        Use Refresh at end of update to ensure that screen is refreshed (core 0)
        /// @param core The core number that is being updated with (0..3)
        UpdateFunction UpdateCore = NULL;

        typedef void (*RenderFunction)(App*, Graphics);
        /// @brief Renders the app on-screen
        /// @param gfx The graphics to render with
        RenderFunction Render = NULL;
};

#endif