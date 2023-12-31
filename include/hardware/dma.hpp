#ifndef _H_DMA
#define _H_DMA
#include <system.hpp>
#include <commons.hpp>
#define DMA_CHANNEL_COUNT 15
/// @brief The minimum amount of bytes for a transfer to be 'efficient'
#define DMA_MINIMUM_TRANSFER 10240

struct alignas(256) DMAControlBlock {
    public:
        /// @brief The transfer information of the DMA transfer
        uint transferInformation = (1 << 4) | (1 << 8);

        /// @brief The source address of the transfer
        uint sourceAddress = NULL;

        /// @brief The destination address of the transfer
        uint destAddress = NULL;
        
        /// @brief The length of the transfer (if using 2D stride then refer to datasheet)
        uint transferLength = 0;

        /// @brief The offset of each row (in 2D stride mode, refer to dataheet)
        uint stride = 0;

        /// @brief If specified, the DMA is to immediately process the next control block.
        uint next = NULL;

        /// @brief Leave as is
        uint debug = 0;

        /// @brief Leave as is
        uint reserved = 0; 

        /// @brief Constructs an cleared control block
        DMAControlBlock();

        /// @brief Enables generating an interrupt when the transfer is complete.
        void EnableInterrupt();

        /// @brief Enables 2D transfer (see datasheet). Useful for offset between data.
        void Enable2DStride();

        /// @brief Ignores the source address and zeroes out the destination.
        void EnableZeroFill();
};

static DMAControlBlock dmaControlBlocks[DMA_CHANNEL_COUNT];
struct DMAManager {
    public:

        DMAManager() = delete;

        /// @brief Initialises the DMA channels
        static void Init();

        /// @brief Transfers the given data using the given DMA channel.
        /// @param channelNumber The number of the DMA channel
        /// @param source The source of the information
        /// @param dest The destination of the copied information
        /// @param length The length of the information to be copied in bytes
        static void Transfer(int channelNumber, 
            uint *source, uint *dest, int length);

        /// @brief Transfers the given data using the default DMA channel (0).
        /// @param source The source of the information
        /// @param dest The destination of the copied information
        /// @param length The length of the information to be copied in bytes
        static void Transfer(uint *source, uint *dest, 
            int length);

            /// @brief Fills the specified data with zeroes using the given DMA channel.
        /// @param channelNumber The number of the DMA channel
        /// @param dest The destination of the zeroes
        /// @param length The number of bytes to zero
        static void Zero(int channelNumber, uint *dest, int length);

        /// @brief Fills the specified data with zeroes using the default DMA channel (0).
        /// @param dest The destination of the copied information
        /// @param length The length of the information to be copied in bytes
        static void Zero(uint *dest, 
            int length);
};
#endif