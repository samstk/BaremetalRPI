#include <hardware/gpio.hpp> 
#include <hardware/framebuffer.hpp>
#include <gfx/gfx.hpp>
#include <resources/systemfont.hpp>
#include <hardware/mailbox.hpp>
#include <data/time.hpp>

/// @brief The main function for the first core
extern "C" void main() {
    // Initialise the board information
    initSystemInfo();

    // Initialise the framebuffer for screen drawing
    Framebuffer framebuffer = Framebuffer(640, 480);
    
    // Initialise the system font
    _systemFont = PSF2Font((char*) _systemFontBuffer);

    // Creates a graphics object for the framebuffer.
    Graphics gfx = Graphics(framebuffer);

    gfx.Fill(Color(32, 32, 32));

    GPIOHandle blinker[4];
    blinker[0] = GPIOHandle(22);
    blinker[1] = GPIOHandle(23);
    blinker[2] = GPIOHandle(24);
    blinker[3] = GPIOHandle(20);

    GPIOHandle input(21);
        
    input.SetInput();
        
    for(int i = 0; i < 4; i++) {
        blinker[i].SetOutput();
    }

    
    while(true) {
        TimeSpan currentTime = TimeSpan::GetCurrentTime();

        for(int i = 0; i < 4; i++) {
            blinker[i].Write(input.IsActive());
        }

        gfx.FillRectangle(Rectangle(16, 16, 16*48, 40), Color(32, 32, 32));

        String timeString = 
            String::ParseLong(currentTime.ticks, StringConversionFormat::ORIGINAL);

        String memoryString = String::ParseLong(
            (unsigned long)timeString.data, StringConversionFormat::HEX
        );

        gfx.DrawString(
            &_systemFont, 
            Point(16, 16), 
            String("Time: ") + timeString,
            Color(255,255,255)
            );

        gfx.DrawString(
            &_systemFont, 
            Point(16, 36), 
            String("Memory Position: ") + memoryString,
            Color(255,255,255)
            );
    }
}

/// @brief The main function for the second core
extern "C" void main_core_2() {
    while(1);
}

/// @brief The main function for the third core
extern "C" void main_core_3() {
    while(1);
}

/// @brief The main function for the fourth core
extern "C" void main_core_4() {
    while(1);
}