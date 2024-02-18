#include <hardware/dma.hpp>
#include <system.hpp>
#include <data/time.hpp>
#include <commons.hpp>
#define DMA_CHANNELS_BASE 0x7000
#define DMA_CHANNELS_ENABLE 0xFF0
#define DMA_CHANNELS_OFFSET 0x100
#define DMA_CHANNEL_COUNT 15


#pragma region struct DMAControlBlock
DMAControlBlock::DMAControlBlock() { }

void DMAControlBlock::Enable2DStride() {
    this->transferInformation |= (1 << 1);
}

void DMAControlBlock::EnableInterrupt() {
    this->transferInformation |= 1;
}

void DMAControlBlock::EnableZeroFill() {
    this->transferInformation |= (1 << 11);
}
#pragma endregion

#pragma region DMAManager
/// @brief A DMA channel register structure.
struct DMAChannel {
    /// @brief The control status register
    uint controlStatus;

    /// @brief The address of the control block
    uint controlBlockAddress;

    /// @brief The loaded block data
    DMAControlBlock loadedBlock;

    /// @brief The debug data
    uint debug;

    DMAChannel() = delete;

    /// @brief Sets the control block
    /// @param block The control block to be loaded into the channel
    void SetControlBlock(DMAControlBlock* block) {
        this->controlBlockAddress = (uint)(ulong)block;
    }

    /// @brief Starts the transfer
    void Start() {
        this->controlStatus |= 1;
    }

    /// @brief Resets the channel and adds a small delay
    void Reset() {
        this->controlStatus |= (1 << 31);
        
        TimeSpan(15, TimeUnit::MICROSECOND).SpinWait();
    }

    /// @brief Determines whether the transfer is finished.
    /// @returns True if the transfer is finished.
    bool IsComplete() {
        return (this->controlStatus & (1 << 1)) != 0;
    }

    /// @brief Determines whether there was an error with the DMA
    /// @return True if an error exists in the DMA engine.
    bool HasError() {
        return (this->controlStatus & (1 << 1)) != 0;
    }
};

void DMAManager::Init() {
    uint* dmaEnableRegister = 
        (uint*) (PERIPHERAL_BASE + DMA_CHANNELS_BASE + DMA_CHANNELS_ENABLE);
    // Enable everything up to DMA engine 14.
    *dmaEnableRegister = 0x7FFF;
}

void DMAManager::Transfer(int channelNumber, 
    uint *source, uint *dest, int length) {

    #ifdef DISABLE_DMA
    for(int i = 0; i < length; i++) {
        *dest = *source;
        source++;
        dest++;
    }
    return;
    #endif

    DMAControlBlock block = dmaControlBlocks[channelNumber] = DMAControlBlock();
    block.sourceAddress = (uint)(ulong)source;
    block.destAddress = (uint)(ulong)dest;
    block.transferLength = length;

    DMAChannel* channel = 
        (DMAChannel*) (ulong) 
        (PERIPHERAL_BASE + DMA_CHANNELS_BASE + channelNumber * DMA_CHANNELS_OFFSET);
    channel->Reset();
    channel->SetControlBlock(&block);
    channel->Start();
    
    TimeSpan currentTime = TimeSpan::GetCurrentTime();
    TimeSpan crashTime = currentTime.AddSeconds(2);
    
    while (!channel->IsComplete()) { 
        currentTime = TimeSpan::GetCurrentTime();
        if (currentTime.ticks >= crashTime.ticks) {
            crash(
                String("DMA Engine (Channel ")
                + String::ParseInt(channelNumber)
                + String(") timed out")
            );
        }

        if (channel->HasError()) {
            if (channel->debug & (1 << 0)) {
                 crash(
                    String("DMA Engine (Channel ")
                    + String::ParseInt(channelNumber)
                    + String(") AXI signal expected")
                );
            } else if (channel->debug & (1 << 1)) {
                crash(
                    String("DMA Engine (Channel ")
                    + String::ParseInt(channelNumber)
                    + String(") FIFO error")
                );
            } else if (channel->debug & (1 << 2)) {
                crash(
                    String("DMA Engine (Channel ")
                    + String::ParseInt(channelNumber)
                    + String(") Slave error")
                );
            } else {
                crash(
                    String("DMA Engine (Channel ")
                    + String::ParseInt(channelNumber)
                    + String(") unknown error")
                );
            }
        }
    }
}

void DMAManager::Transfer(uint *source, uint *dest, 
    int length) {
    // Default to using channel 0.
    DMAManager::Transfer(0, source, dest, length);
}

void DMAManager::Zero(int channelNumber, uint *dest, int length) {
    DMAControlBlock block = dmaControlBlocks[channelNumber] = DMAControlBlock();
    block.sourceAddress = 0;
    block.destAddress = (uint)(ulong)dest;
    block.transferLength = length;
    block.EnableZeroFill();

    DMAChannel* channel = 
        (DMAChannel*) (ulong) 
        (PERIPHERAL_BASE + DMA_CHANNELS_BASE + channelNumber * DMA_CHANNELS_OFFSET);

    channel->Reset();
    channel->SetControlBlock(&block);
    channel->Start();

    while (!channel->IsComplete()) { }
}

void DMAManager::Zero(uint *dest, 
    int length) {
    // Default to using channel 0.
    DMAManager::Zero(0, dest, length);
}
#pragma endregion