#include <hardware/gpio.hpp> 
#include <hardware/framebuffer.hpp>
#include <gfx/gfx.hpp>
#include <resources/systemfont.hpp>
#include <hardware/mailbox.hpp>

/// @brief The main function for the first core
extern "C" void main() {
    // Initialise the framebuffer for screen drawing
    Framebuffer framebuffer = Framebuffer(640, 480);
    
    // Initialise the system font
    _systemFont = PSF2Font((char*) _systemFontBuffer);

    // Creates a graphics object for the framebuffer.
    Graphics gfx = Graphics(framebuffer);

    // Fill blackish background
    gfx.Fill(Color(32, 32, 32));
        
    // Draw whitish rectangle
    gfx.FillRectangle(Rectangle(122, 122, 120, 64), Color(165, 165, 165));

    // Draw white text
    gfx.DrawString(&_systemFont, Point(16, 16), (char*) "Hello World!", Color(255,255,255));

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
        for(int i = 0; i < 4; i++) {
            blinker[i].Write(input.IsActive());
        }
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