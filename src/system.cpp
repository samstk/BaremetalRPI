#include <system.hpp>
#include <hardware/mailbox.hpp>
#include <data/string.hpp>
#include <hardware/framebuffer.hpp>
#include <gfx/gfx.hpp>
#include <gfx/font.hpp>
#include <resources/systemfont.hpp>
#include <resources/funcmap.hpp>

/// @brief Metadata for memory allocation
struct MemoryAllocationMetadata {
    public:
        /// @brief The size of the allocation
        int size;
        /// @brief True if the metadata represents allocated memory 
        int allocated;
        /// @brief True if the next allocation metadata has already been accessed.
        int nextAllocationAccessed;
};

#pragma region struct RPIBoard
RPIMachineInfo::RPIMachineInfo() {
    this->memoryStart = (char*) &_end;
    // 512 megabytes (temporary value) - stack
    this->memoryRAM = 0x1C000000 - 0x400000 - (unsigned long)this->memoryStart;
    
    this->memoryEnd = this->memoryStart + this->memoryRAM;
    this->memoryTarget = this->memoryStart;

    MemoryAllocationMetadata* startMetadata = 
        (MemoryAllocationMetadata*)this->memoryStart;
    startMetadata->size = this->memoryRAM;
    startMetadata->allocated = false;
    startMetadata->nextAllocationAccessed = false;
}
#pragma endregion

#pragma region class Stacktrace
struct Stackframe {
    Stackframe* ebp;
    unsigned int eip;
};

Stacktrace::Stacktrace() {
    struct Stackframe *stk;
    asm volatile("mov %0, sp" : "=r"(stk));
    
    // Clear address references
    for(int i = 0; i < SYSTEM_STACKTRACE_MAXSTEPS; i++) {
        this->stack[i] = NULL;
    }

    // Travel through stack frames
    int index = 0;
    this->stack[index++] = stk->eip - 4;

    while(stk->ebp && index < SYSTEM_STACKTRACE_MAXSTEPS) {
        stk = stk->ebp;
        this->stack[index++] = stk->eip - 4;
    }
}
#pragma endregion

static RPIMachineInfo _systemInfo;

void initSystemInfo() {
    _systemInfo = RPIMachineInfo();
}

RPIMachineInfo getSystemInfo() {
    return _systemInfo;
}

int getCoreId() {
    register int coreId;
    asm volatile("mrs %0, mpidr_el1" : "=r"(coreId));
    return coreId & 0b11;
}

#pragma region struct FunctionLabel
FunctionLabel::FunctionLabel() {
    this->name = String();
    this->address = NULL;
}

FunctionLabel::FunctionLabel(unsigned int programAddress) {
    char* funcmap = getFunctionMap();

    // All definitions are in the form of:
    // XXXXXXXXXXXXXXXX <funcname>:

    // Continue to check until there are no longer any valid pointers.

    char* prevLabel = funcmap;
    unsigned long prevAddress = String(funcmap, 16).ToUInt64(StringConversionFormat::HEX);
    
    while (*funcmap == '0') {
        String location = String(funcmap, 16);
        
        unsigned long address = location.ToUInt64(StringConversionFormat::HEX);
        
        if (programAddress <= address) {
            this->address = prevAddress;
            funcmap = prevLabel;
            funcmap += 18;
            int length = 0;
            while (*funcmap != '>') {
                length++;
                funcmap++;
            }
            this->name = String(prevLabel+18, length);
            return;
        }

        prevLabel = funcmap;
        prevAddress = address;

        // Skip to next entry.
        while (*funcmap != '\n') {
            funcmap++;
        }    
        funcmap++;
    }

    this->name = "unknown";
    this->address = programAddress;
}
#pragma endregion

/// @brief Crashes the system if the asserted value is not true.
void assert(bool asserted, const char* label) {
    if (!asserted) {
        if (label) {
            crash(String("Assertion failed -- ") + String(label));
        } else {
            crash("Assertion failed");
        }
    }
}

#define MALLOC_METADATASIZE sizeof(MemoryAllocationMetadata)

extern "C" void* malloc(int size) {
    // Simple implementation of malloc (slow but simple, temporary solution only)
    //
    // Explanation:
    // M = Metadata about the allocation.
    // D = data that can be written and is returned from malloc.
    // Structure: [M][DDDD][M][DDDD]
    //
    // When the memory is first initialised the structure is just below:
    // [M size=1000]
    //
    // Every-time malloc is called it searches for the first available allocation.
    // [M] <- is free
    // [M size=4][DDDD][M size=996 (new metadata)]
    // 
    // Say the first element is freed
    // [M size=4 free][DDDD][M size=996]
    //
    // Then the next allocation (of size 4 or below) will go to the first element.

    // Round size up (align 4) so that everything is aligned to 32-bits.
    size = ((size + 3) / 4) * 4;

    RPIMachineInfo systemInfo = getSystemInfo();

    // Find the next available allocation
    MemoryAllocationMetadata* metadata = (MemoryAllocationMetadata*) systemInfo.memoryStart;

    while(metadata->allocated || metadata->size < size) {
        metadata = (MemoryAllocationMetadata*) 
            ((char*) metadata + MALLOC_METADATASIZE + metadata->size);

        if ((void*) metadata >= systemInfo.memoryEnd) {
            crash("Ran out of Memory");
        }
    }

    // Mark allocation as allocated
    int oldSize = metadata->size;
    metadata->allocated = true;

    // Initialise next metadata if necessary.
    if (!metadata->nextAllocationAccessed) {
        // 'Fragment' the allocation as new allocation data can be
        // added directly after (with new size as leftover size from this allocation)
        MemoryAllocationMetadata* nextMetadata = (MemoryAllocationMetadata*) 
        ((char*) metadata + MALLOC_METADATASIZE + size);
        metadata->nextAllocationAccessed = true;
        metadata->size = size;
        nextMetadata->nextAllocationAccessed = false;
        nextMetadata->size = oldSize - size;
        nextMetadata->allocated = false;
    }

    return (void*) ((char*) metadata + MALLOC_METADATASIZE);
}

extern "C" void free(void* alloc) {
    // Assumes the block is directly behind the allocation
    MemoryAllocationMetadata* metadata = 
        (MemoryAllocationMetadata*) ((char*) alloc - MALLOC_METADATASIZE);

    metadata->allocated = false;
}

void setSystemFramebuffer(Framebuffer buffer) {
    _systemFramebuffer = buffer;
}

Framebuffer getSystemFramebuffer() {
    return _systemFramebuffer;
}

void crash() {
    crash("Unknown error occurred");
}

void crash(String errorMessage) {
    Framebuffer buffer = getSystemFramebuffer();
    Graphics crashGfx = Graphics(buffer);
    crashGfx.Fill(Color(0, 0, 0));
    PSF2Font systemFont = getSystemFont();
    int y = 16;
    Color white = Color(255,255,255);
    
    crashGfx.DrawString(&systemFont, Point(16, y), String("Fatal Error: ") + errorMessage, 
        white);
    
    y += 20;
    unsigned int coreId = getCoreId();
    String coreDiagnostic = String("Executing Core ID: ") + String::ParseInt(coreId);
    crashGfx.DrawString(&systemFont, Point(16, y), coreDiagnostic, white);

    y += 20;
    crashGfx.DrawString(&systemFont, Point(16, y), "Call Stack:", white);

    String at = "@ : ";

    Stacktrace trace = Stacktrace();

    for(int i = 0; i < SYSTEM_STACKTRACE_MAXSTEPS && i < 12; i++) {
        y += 20;

        unsigned int address = trace.stack[i];

        if (address == NULL) {
            break;
        }

        FunctionLabel label = FunctionLabel(address);
        String printedString = at + String::ParseInt(address, StringConversionFormat::HEX);
        String printedString2 = printedString + (String(" <") + label.name + String("+"));
        
        unsigned int offset = address - label.address;
        String printedString3 = printedString2
            + String::ParseInt(offset, StringConversionFormat::HEX)
                .Substring(4) // It is expected that a function does not exceed 0xFFFF. 
            + String(">");

        crashGfx.DrawString(&systemFont, Point(16, y), 
            printedString3, 
            white);
    }

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
