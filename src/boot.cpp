#include <hardware/gpio.hpp> 

/// @brief The main function for the first core
extern "C" void main() {
    // Test GPIO Logic by flashing LEDs
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