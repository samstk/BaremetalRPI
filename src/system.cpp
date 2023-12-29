#include <system.hpp>
#include <hardware/mailbox.hpp>

/// @brief Metadata for memory allocation
struct MemoryAllocationMetadata {
    public:
        /// @brief The size of the allocation
        int size;
        /// @brief True if the metadata represents allocated memory 
        bool allocated;
        /// @brief True if the next allocation metadata has already been accessed.
        bool nextAllocationAccessed;
};

#pragma region struct RPIBoard
RPIBoard::RPIBoard() {
    this->memoryRAM = 0x20000000; // 512 megabytes (temporary value)
    this->memoryStart = (char*) &_end;
    this->memoryEnd = this->memoryStart + this->memoryRAM;
    this->memoryTarget = this->memoryStart;

    MemoryAllocationMetadata* startMetadata = 
        (MemoryAllocationMetadata*)this->memoryStart;
    startMetadata->size = this->memoryRAM;
    startMetadata->allocated = false;
    startMetadata->nextAllocationAccessed = false;
}
#pragma endregion

#define MALLOC_METADATASIZE 4 

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

    // Find the next available allocation
    MemoryAllocationMetadata* metadata = (MemoryAllocationMetadata*) _systemBoard.memoryStart;
    
    while(metadata->allocated || metadata->size < size) {
        metadata = (MemoryAllocationMetadata*) 
            ((char*) metadata + MALLOC_METADATASIZE + metadata->size);

        if ((void*) metadata >= _systemBoard.memoryEnd)
            return NULL;
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
