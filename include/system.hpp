#ifndef _H_SYSTEM
#define _H_SYSTEM
#define NULL 0

/// @brief Throws an unknown error
void crash();

/// @brief Swaps the endianness of an 32-bit value
unsigned int magic(unsigned int value);

/// @brief Copies a number of bytes from src to dest
/// @param dest The destination of the bytes
/// @param src The source of the bytes (to copy from)
/// @param count The number of bytes
extern "C" void memcpy(void* dest, const void* src, unsigned long count);

#endif