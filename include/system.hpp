#ifndef _H_SYSTEM
#define _H_SYSTEM
#define NULL 0
#define PERIPHERAL_BASE       0x3F000000

/// @brief A structure containing all relevant information of the board
struct RPIMachineInfo {
    public:
        /// @brief The amount of RAM available
        int memoryRAM;
        
        /// @brief The starting address for freely available memory
        char* memoryStart;

        /// @brief The end address for freely available memory
        char* memoryEnd;
        
        /// @brief The next address for allocated memory.
        char* memoryTarget;

        /// @brief Initialises the information of the board
        RPIMachineInfo();
};

/// @brief Initialises the system board information
void initSystemInfo();

/// @brief Gets the system board information
RPIMachineInfo getSystemInfo();

/// @brief Reference to the label prior to free memory
extern "C" void* _end;

/// @brief Allocates contiguous memory 
/// @param size The size of the memory to be allocated.
/// @return The address in which the memory can be allocated.
extern "C" void* malloc(int size);

/// @brief Frees the allocated memory
/// @param alloc The memory that was allocated.
extern "C" void free(void* alloc);

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