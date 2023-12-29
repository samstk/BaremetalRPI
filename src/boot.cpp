#include <hardware/gpio.hpp> 
#include <hardware/framebuffer.hpp>
#include <gfx/gfx.hpp>
#include <resources/systemfont.hpp>
#include <hardware/mailbox.hpp>

/// @brief The main function for the first core
extern "C" void main() {
    // Initialise the board information
    _systemBoard = RPIBoard();

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

    int time = 0;
    while(true) {
        for(int i = 0; i < 4; i++) {
            blinker[i].Write(input.IsActive());
        }

        gfx.FillRectangle(Rectangle(16, 16, 16*32, 20), Color(32, 32, 32));

        gfx.DrawString(
            &_systemFont, 
            Point(16, 16), 
            String::ParseInt(time, StringConversionFormat::BINARY), 
            Color(255,255,255)
            );

        time++;
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