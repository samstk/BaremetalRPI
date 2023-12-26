#include <hardware/gpio.hpp> 

/// @brief The main function for the first core
extern "C" void main() {
    // Test GPIO Logic by flashing LEDs
    GPIOHandle user(22);
    GPIOHandle pin2(29);

    user.SetOutput();
    pin2.SetOutput();

    while(true) {
        user.Write(true);
        pin2.Write(false);

        // For now since we don't have timers set up
        // just use iterations to introduce a pause
        for(volatile int i = 0; i < 1000000; i++) {
            __asm__("nop");
        }

        user.Write(false);
        pin2.Write(true);
        
        for(volatile int i = 0; i < 2000000; i++) {
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