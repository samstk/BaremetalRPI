#include <system.hpp>

void crash() {
    while (1);
}

unsigned int magic(unsigned int value) {
    return ((value & 0xFF) << 24)
        | ((value & 0xFF00) << 8)
        | ((value & 0xFF0000) >> 8)
        | ((value & 0xFF000000) >> 24);
}

extern "C" void memcpy(void* dest, const void* src, unsigned long count) {
    char* copyTo = (char*)dest;
    char* copyFrom = (char*)src;

    while(count > 0) {
        *copyTo = *copyFrom;
        copyTo++;
        copyFrom++;
        count--;
    }
}
