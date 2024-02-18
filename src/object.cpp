#include <system.hpp>
void* operator new(unsigned long size) {
    return malloc((int)size);
}

void operator delete(void* position) {
    free(position);
}