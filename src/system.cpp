#include <system.hpp>
#include <hardware/mailbox.hpp>
#include <data/string.hpp>
#include <hardware/framebuffer.hpp>
#include <gfx/gfx.hpp>
#include <gfx/font.hpp>
#include <resources/systemfont.hpp>
#include <resources/funcmap.hpp>
#include <commons.hpp>
#include <hardware/dma.hpp>

/// @brief Metadata for memory allocation
struct MemoryAllocationMetadata {
    public:
        /// @brief The size of the allocation
        int size;

        /// @brief True if the metadata represents allocated memory 
        int allocated;
        
        /// @brief The address of the previous metadata (NULL if none)
        uint previous;

        /// @brief The address of the next metadata (NULL if none)
        uint next;
};

#define USE_DMA_FOR_MEMCPY true
#define MALLOC_METADATASIZE sizeof(MemoryAllocationMetadata)

#pragma region struct RPIBoard
RPIMachineInfo::RPIMachineInfo() {
    this->memoryStart = (char*) &_end;
    this->memoryRAM = 0x1C000000 - 0x400000 - (ulong)this->memoryStart
        - MALLOC_METADATASIZE;
    this->memoryEnd = this->memoryStart + this->memoryRAM;
}
#pragma endregion

#pragma region class Stacktrace
struct Stackframe {
    Stackframe* ebp;
    uint eip;
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
static int allocations = 0;

/// @brief The next allocation to be considered in malloc
static MemoryAllocationMetadata* nextAllocation;

void initSystemInfo() {
    _systemInfo = RPIMachineInfo();
    allocations = 0;

    nextAllocation = (MemoryAllocationMetadata*) _systemInfo.memoryStart;
    nextAllocation->previous = NULL;
    nextAllocation->next = NULL;
    nextAllocation->allocated = false;
    nextAllocation->size = _systemInfo.memoryRAM;
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

FunctionLabel::FunctionLabel(uint programAddress) {
    char* funcmap = getFunctionMap();

    // All definitions are in the form of:
    // XXXXXXXXXXXXXXXX <funcname>:

    // Continue to check until there are no longer any valid pointers.

    char* prevLabel = funcmap;
    ulong prevAddress = String(funcmap, 16).ToUInt64(StringConversionFormat::HEX);
    
    while (*funcmap == '0') {
        String location = String(funcmap, 16);
        
        ulong address = location.ToUInt64(StringConversionFormat::HEX);
        
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

extern "C" void* malloc(int size) {
    // Simple implementation of malloc
    //
    // Explanation:
    // [M->][DDDD][<-M->][DDDD][<-M->][DDDD]
    //
    // A pointer to the end of the list is kept statically,
    // and is considered first. If the allocation is not suitable,
    // then it will search around the list for a suitable allocation.
    // (one that is not allocated and is at least the size wanted)
    // 
    // An error occurs if the maximum amount of memory has been reached,
    // and all allocations have been checked for free memory.

    // Round size up (align 4) so that everything is aligned to 32-bits.
    size = ((size + 3) / 4) * 4;
    
    RPIMachineInfo systemInfo = getSystemInfo();
    allocations++;
    
    // Find the next available allocation
    MemoryAllocationMetadata* metadata = nextAllocation;

    while (metadata->allocated || metadata->size < size) {
        // Move to the next metadata in the list
        if (metadata->next == NULL) {
            // Must be at the end of the list so restart
            metadata = (MemoryAllocationMetadata*) systemInfo.memoryStart;
        }
        else {
            metadata = (MemoryAllocationMetadata*) (ulong) metadata->next;
        }

        // Check whether we have looped through the entire list
        // was unable to find a suitable allocation
        if (metadata == nextAllocation) {
            initSystemInfo(); // Reset memory
            crash(String("No Memory (REQ: ")
                + String::ParseInt(size)
                + String(", RAM: ") 
                + String::ParseInt((uint)systemInfo.memoryRAM, StringConversionFormat::HEX)
                + String(", N=")
                + String::ParseInt(allocations - 1)
                + String(")"));
        }
    }

    // A suitable allocation was found so reserve it.
    metadata->allocated = true;
    
    // In order to keep memory functional and as we are at the
    // end of the list, we need to ensure that we always
    // have more allocations available by 'fragmenting' this allocation.
    if (metadata->next == NULL) {
        if ((ulong)metadata + size + MALLOC_METADATASIZE < (ulong)systemInfo.memoryEnd) {
            // Resize current allocation to appropriate size.
            int oldSize = metadata->size;
            metadata->size = size;

            // Reset next metadata and set size of allocation.
            metadata->next = (uint) 
                ((ulong)metadata + MALLOC_METADATASIZE + size);

            MemoryAllocationMetadata *nextMetadata = 
                (MemoryAllocationMetadata *) (ulong) metadata->next;
        
            nextMetadata->previous = (uint) (ulong)metadata;
            nextMetadata->allocated = false;
            nextMetadata->next = NULL;
            nextMetadata->size = oldSize - size - MALLOC_METADATASIZE;
        }
    }

    // Keep reference to the next allocation which may be free.
    if (metadata->next != NULL) {
        nextAllocation = (MemoryAllocationMetadata*) (ulong) metadata->next;
    }

    return (void*) ((char*) metadata + MALLOC_METADATASIZE);
}

extern "C" void free(void* alloc) {
    // Assumes the block is directly behind the allocation
    MemoryAllocationMetadata* metadata = 
        (MemoryAllocationMetadata*) ((char*) alloc - MALLOC_METADATASIZE);

    metadata->allocated = false;
}

extern "C" void __cxa_pure_virtual() {
    crash("A pure virtual function was called");
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
    uint coreId = getCoreId();
    String coreDiagnostic = String("Executing Core ID: ") + String::ParseInt(coreId);
    crashGfx.DrawString(&systemFont, Point(16, y), coreDiagnostic, white);

    y += 20;
    crashGfx.DrawString(&systemFont, Point(16, y), "Call Stack:", white);

    String at = "@ : ";

    Stacktrace trace = Stacktrace();

    for(int i = 0; i < SYSTEM_STACKTRACE_MAXSTEPS && i < 12; i++) {
        y += 20;

        uint address = trace.stack[i];

        if (address == NULL) {
            break;
        }

        FunctionLabel label = FunctionLabel(address);
        String printedString = at + String::ParseInt(address, StringConversionFormat::HEX);
        String printedString2 = printedString + (String(" <") + label.name + String("+"));
        
        uint offset = address - label.address;
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

uint magic(uint value) {
    return ((value & 0xFF) << 24)
        | ((value & 0xFF00) << 8)
        | ((value & 0xFF0000) >> 8)
        | ((value & 0xFF000000) >> 24);
}

extern "C" void memcpy(void* dest, const void* src, ulong count) {
    char* copyTo = (char*)dest;
    char* copyFrom = (char*)src;

    if (count >= DMA_MINIMUM_TRANSFER && ((ulong) src % 4 == 0)
        && ((ulong) dest % 4 == 0) && USE_DMA_FOR_MEMCPY) {
        // Must use DMA to prevent CPU wastage from memcpy
        // however the locations must be aligned to 32-bit
        ulong words = count / 4;
        ulong bytes = count - words * 4; 
        DMAManager::Transfer((uint *)src, (uint *)dest, (int)(words * 4));
        
        // Adjust manual copy amount to bytes and copy address
        int adjustment = count - bytes;
        copyTo += adjustment;
        copyFrom += adjustment;
        count = bytes;
    } 

    while(count > 0) {
            *copyTo = *copyFrom;
            copyTo++;
            copyFrom++;
            count--;
    }
}
