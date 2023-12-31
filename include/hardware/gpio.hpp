#ifndef _H_GPIO
#define _H_GPIO
#include <commons.hpp>
#include <system.hpp>
#define GPFSEL0         ((uint*)(PERIPHERAL_BASE+0x00200000))
#define GPFSEL1         ((uint*)(PERIPHERAL_BASE+0x00200004))
#define GPFSEL2         ((uint*)(PERIPHERAL_BASE+0x00200008))
#define GPFSEL3         ((uint*)(PERIPHERAL_BASE+0x0020000C))
#define GPFSEL4         ((uint*)(PERIPHERAL_BASE+0x00200010))
#define GPFSEL5         ((uint*)(PERIPHERAL_BASE+0x00200014))
#define GPSET0          ((uint*)(PERIPHERAL_BASE+0x0020001C))
#define GPSET1          ((uint*)(PERIPHERAL_BASE+0x00200020))
#define GPCLR0          ((uint*)(PERIPHERAL_BASE+0x00200028))
#define GPCLR1          ((uint*)(PERIPHERAL_BASE+0x0020002C))
#define GPLEV0          ((uint*)(PERIPHERAL_BASE+0x00200034))
#define GPLEV1          ((uint*)(PERIPHERAL_BASE+0x00200038))

/// @brief A handle for a GPIO Pin on the Board
///        which contains information and functions relevant
///        to the fundamental function of each pin
struct GPIOHandle {
    public:
        /// @brief The pin number on the board
        int pinNumber; 

    private:
        /// @brief The function select address used for this pin
        uint* functionSelectAddress;

        /// @brief The output set address used for this pin
        uint* outputSetAddress;

        /// @brief The output clear address used for this pin
        uint* outputClearAddress;

        /// @brief The level address used for this pin
        uint* inputLevelAddress;

        /// @brief The offset in the general registers
        ///        that refers to the pin (bit position)
        uint bitMask;

        /// @brief The offset in the function select register
        ///        that refers to this pin
        uint functionOffset;
        
    public:
        /// @brief Creates an uninitialized handle for a GPIO pin.
        ///        Use SetPin to adjust 
        GPIOHandle();

        /// @brief Initialises the GPIO with reference to the handle
        /// @param pinNumber The pin number on the board (e.g. GPIO 22)
        GPIOHandle(int pinNumber);

        /// @brief Initialises the reference to the GPIO pin.
        /// @param pinNumber The pin number of the board (e.g. GPIO 22)
        void SetPin(int pinNumber);

        /// @brief Sets the pin to Input Mode
        void SetInput();

        /// @brief Sets the pin to Output Mode
        void SetOutput();

        /// @brief Writes either HIGH or LOW to the Output Pin.
        /// @param high If true, then the output pin is set high, else low.
        void Write(bool high);

        /// @brief Reads either HIGH (true) or LOW (false) from the Input Pin.
        bool IsActive();
};

#endif