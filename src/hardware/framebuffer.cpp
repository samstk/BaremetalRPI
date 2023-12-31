#include <hardware/framebuffer.hpp>
#include <hardware/mailbox.hpp>
#include <commons.hpp>
#pragma region struct Framebuffer
#define PROP_PHYSICAL_DISPLAY   0x00048003
#define PROP_VIRTUAL_BUFFER     0x00048004
#define PROP_DEPTH              0x00048005
#define PROP_VIRTUAL_OFFSET     0x00048009
#define PROP_PALETTE            0x0004800B
#define PROP_ALLOCATE_BUFFER    0x00040001

Framebuffer::Framebuffer() {
    
}

Framebuffer::Framebuffer(int width, int height) {
    this->resolution = Size(width, height);

    // Write the mailbox data until we have received a physical address for the 
    // framebuffer.
    Mailbox mailbox = Mailbox();
    
    mailbox.Reset();
    
    // Add general configuration
    mailbox.AddRequestProperty(PROP_PHYSICAL_DISPLAY,
        (uint)width, (uint)height);
    mailbox.AddRequestProperty(PROP_VIRTUAL_BUFFER, 
        (uint)width, (uint)height);

    mailbox.AddRequestProperty(PROP_DEPTH, 32);
    mailbox.AddRequestProperty(PROP_VIRTUAL_OFFSET, 0, 0);
    mailbox.AddRequestProperty(PROP_PALETTE, 
        0, 2, 0x00000000, 0xFFFFFFFF);
    
    // Add response buffer address (64-bit)
    uint *buffer_pointer = (uint *)mailbox.AddRequestProperty(
        PROP_ALLOCATE_BUFFER, 0, 0
    );

    mailbox.Pack();
    
    while (true) {
        mailbox.Write();
        
        uint fbPointer = *buffer_pointer;
        if (fbPointer != 0) {
            // Take conversion from BUS address to physical address
            this->buffer = (uint *)((ulong)(fbPointer & 0x3FFFFFFF));
            return;
        }
    }
}
#pragma endregion