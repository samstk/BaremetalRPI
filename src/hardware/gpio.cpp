#include <hardware/gpio.hpp>

#pragma region struct GPIOHandle
#define GPFCLEARMASK 0b111
#define GPFOUTPUTMASK 0b001
#define GPFALT0MASK 0b100
#define GPFALT1MASK 0b101
#define GPFALT2MASK 0b110
#define GPFALT3MASK 0b111
#define GPFALT4MASK 0b011
#define GPFALT5MASK 0b010

GPIOHandle::GPIOHandle(int pinNumber) {
    this->pinNumber = pinNumber;
    
    // Determine function select address
    if (pinNumber <= 9) {
        this->functionSelectAddress = GPFSEL0;
        this->functionOffset = (pinNumber) * 3;
    } else if (pinNumber <= 19) {
        this->functionSelectAddress = GPFSEL1;
        this->functionOffset = (pinNumber - 10) * 3;
    } else if (pinNumber <= 29) {
        this->functionSelectAddress = GPFSEL2;
        this->functionOffset = (pinNumber - 20) * 3;
    } else if (pinNumber <= 39) {
        this->functionSelectAddress = GPFSEL3;
        this->functionOffset = (pinNumber - 30) * 3;
    } else if (pinNumber <= 49) {
        this->functionSelectAddress = GPFSEL4;
        this->functionOffset = (pinNumber - 40) * 3;
    } else if (pinNumber <= 53) {
        this->functionSelectAddress = GPFSEL5;
        this->functionOffset = (pinNumber - 50) * 3;
    }

    // Determine alternate registers
    if (pinNumber <= 31) {
        this->outputSetAddress = GPSET0;
        this->outputClearAddress = GPCLR0;
        this->inputLevelAddress = GPLEV0;
        this->bitMask = 1 << pinNumber;
    } else {
        this->outputSetAddress = GPSET1;
        this->outputClearAddress = GPCLR1;
        this->inputLevelAddress = GPLEV1;
        this->bitMask = 1 << (pinNumber - 32);
    }
}

void GPIOHandle::SetInput() {
    // Clear pin register value (000 = input)
    *this->functionSelectAddress &= ~(GPFCLEARMASK << this->functionOffset);
}

void GPIOHandle::SetOutput() {
    unsigned int currentFunction = *this->functionSelectAddress;
    currentFunction &= ~(GPFCLEARMASK << this->functionOffset);
    currentFunction |= GPFOUTPUTMASK << this->functionOffset;
    *this->functionSelectAddress = currentFunction;
}

void GPIOHandle::Write(bool high) {
    if (high) {
        *this->outputClearAddress |= this->bitMask;
    } else {
        *this->outputSetAddress |= this->bitMask;
    }
}

bool GPIOHandle::IsActive() {
    if (*this->inputLevelAddress & this->bitMask) {
        return true;
    }
    return false;
}

#pragma endregion 