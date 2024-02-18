#include <hardware/gpio.hpp> 
#include <hardware/framebuffer.hpp>
#include <gfx/gfx.hpp>
#include <resources/systemfont.hpp>
#include <hardware/mailbox.hpp>
#include <data/time.hpp>
#include <system.hpp>
#include <commons.hpp>
#include <hardware/dma.hpp>
#include <app.hpp>
#include <apps/systemapp.hpp>

static App* SysApp = NULL;

/// @brief The main function for the first core
extern "C" void main() {
    // Initialise the DMA engine
    DMAManager::Init();
    
    // Initialise the board information
    initSystemInfo();

    // Initialise the framebuffer for screen drawing
    Framebuffer framebuffer = Framebuffer(1024, 600);
    setSystemFramebuffer(framebuffer); // For error messages

    // Initialise the system font
    _systemFont = PSF2Font((char*) _systemFontBuffer);
    setSystemFont(_systemFont);

    // Creates a graphics object for the framebuffer.
    Graphics gfx = Graphics(framebuffer, true);
    gfx.Fill(Color(22, 22, 22));

    // Setup system app
    SysApp = new SystemApp();
    SysApp->Init(SysApp);

    while(true) {
        SysApp->UpdateCore(SysApp, 0);
        
        if (SysApp->refreshScreen) {
            
            SysApp->refreshScreen = false;
            SysApp->Render(SysApp, gfx);
            gfx.Flush();
        }
    }
}

/// @brief The main function for the second core
extern "C" void main_core_2() {
    while(SysApp == NULL);

    while(1) {
        if (SysApp != NULL) {
            SysApp->UpdateCore(SysApp, 1);
        }
    }
}

/// @brief The main function for the third core
extern "C" void main_core_3() {
    while(SysApp == NULL);

    while(1) {
        if (SysApp != NULL) {
            SysApp->UpdateCore(SysApp, 2);
        }
    }
}

/// @brief The main function for the fourth core
extern "C" void main_core_4() {
    while(SysApp == NULL);

    while(1) {
        if (SysApp != NULL) {
            SysApp->UpdateCore(SysApp, 3);
        }
    }
}