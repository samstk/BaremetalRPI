#include <hardware/gpio.hpp> 

/// @brief The main function for the first core
extern "C" void main() {
    // Test GPIO Logic by flashing LEDs
    GPIOHandle blinker(22);
    GPIOHandle blinker2(23);
    GPIOHandle blinker3(24);                             

    blinker.SetOutput();
    blinker2.SetOutput();
    blinker3.SetOutput();

    while(true) {
        blinker.Write(true);
        blinker2.Write(false);
        blinker3.Write(false);

        // For now since we don't have timers set up
        // just use iterations to introduce a pause
        for(int i = 0; i < 1500000; i++) {
            __asm__("nop");1
        }

        blinker.Write(false);
        blinker2.Write(true);
        blinker3.Write(false);
        for(int i = 0; i < 1500000; i++) {
            __asm__("nop");
        }

        blinker.Write(false);
        blinker2.Write(false);
        blinker3.Write(true);
        for(int i = 0; i < 1500000; i++) {
            __asm__("nop");
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