#ifndef _H_MAILBOX
#define _H_MAILBOX
#include <commons.hpp>
/// @brief The base address for the mailbox interface
#define MAIL_BASE 0xB880

/// @brief The offset address for the write interface
#define MAIL_WRITE 0x20

/// @brief Offset for the tags of a mailbox item
#define MAIL_TAGS 0x8

volatile static uint mbox[1028] __attribute__((aligned(16)));

/// @brief A structure representing the mailbox interface
struct Mailbox {
    /// @brief The IO address of the mailbox interface to write to.
    uint *ioAddress;

    /// @brief The data array (initialized to mbox)
    uint *data;
    
    /// @brief The next offset in the data array
    uint dataOffset = 0;

    /// @brief The number of bytes written.
    uint dataSize = 0;

    /// @brief Initialises a new instance of the mailbox interface
    Mailbox();

    /// @brief Resets the mailbox
    void Reset();

    /// @brief Finalizes the mailbox data
    void Pack();

    /// @brief Adds a single word of data to the mailbox data
    /// @param data The data to add
    /// @return The address of the data
    uint* AddData(uint data);

    /// @brief Adds a property to the mailbox data
    /// @param code The data to add
    /// @param arg1 The first value of the property
    /// @return The starting address of the data
    uint* AddRequestProperty(uint code, uint arg1);

    /// @brief Adds a property to the mailbox data
    /// @param code The data to add
    /// @param arg1 The first value of the property
    /// @param arg2 The second value of the property
    /// @return The starting address of the data
    uint* AddRequestProperty(uint code, uint arg1, uint arg2);

    /// @brief Adds a property to the mailbox data
    /// @param code The data to add
    /// @param arg1 The first value of the property
    /// @param arg2 The second value of the property
    /// @param arg3 The third value of the property
    /// @return The starting address of the data
    uint* AddRequestProperty(uint code, uint arg1, uint arg2,
        uint arg3);

    /// @brief Adds a property to the mailbox data
    /// @param code The data to add
    /// @param arg1 The first value of the property
    /// @param arg2 The second value of the property
    /// @param arg3 The third value of the property
    /// @param arg4 The fourth value of the property
    /// @return The starting address of the data
    uint* AddRequestProperty(uint code, uint arg1, uint arg2,
        uint arg3, uint arg4);

    /// @brief Writes the mailbox data
    void Write();
};
#endif