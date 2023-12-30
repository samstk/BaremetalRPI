#ifndef _H_SYSTEM
#define _H_SYSTEM
#define NULL 0
#define PERIPHERAL_BASE       0x3F000000
#define SYSTEM_STACKTRACE_MAXSTEPS 6
#include <hardware/framebuffer.hpp>
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

/// @brief A stack trace which traces a maximum of 6 steps.
class Stacktrace {
    public:
        /// @brief The current address of this step.
        unsigned int stack[6];

        /// @brief Creates a new stack trace
        Stacktrace();
};

/// @brief Crashes the system if the asserted value is not true.
void assert(bool asserted, const char* label = NULL);

/// @brief Initialises the system board information
void initSystemInfo();

/// @brief Gets the ID of the current core in execution
/// @return The ID of the core
int getCoreId();

/// @brief Gets the system board information
RPIMachineInfo getSystemInfo();

static Framebuffer _systemFramebuffer;

/// @brief Sets the system's framebuffer that is written to when crashed
/// @param buffer The buffer that is written to when crashed.
void setSystemFramebuffer(Framebuffer buffer);

/// @brief Gets the system framebuffer as set in setSystemFramebuffer
/// @return The system framebuffer
Framebuffer getSystemFramebuffer();

/// @brief A label of a function in program memory
struct FunctionLabel {
    public:
        /// @brief The address of the function
        unsigned int address;

        /// @brief The name of the function
        String name;

        /// @brief Constructs an uninitialized function label
        FunctionLabel();

        /// @brief Constructs a new function label approximately from the address given 
        /// @param programAddress The address of a line within the function.
        FunctionLabel(unsigned int programAddress);


};

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

/// @brief Throws an error and crashes all other cores
/// @param errorMessage The error message displayed to the user
void crash(String errorMessage);


/// @brief Swaps the endianness of an 32-bit value
unsigned int magic(unsigned int value);

/// @brief Copies a number of bytes from src to dest
/// @param dest The destination of the bytes
/// @param src The source of the bytes (to copy from)
/// @param count The number of bytes
extern "C" void memcpy(void* dest, const void* src, unsigned long count);

#endif