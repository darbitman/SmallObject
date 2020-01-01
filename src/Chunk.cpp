#include "Chunk.hpp"

#include <cassert>

Chunk::Chunk() noexcept
    : pData_(nullptr), pDataEnd_(nullptr), nextAvailableBlock_(0), blocksAvailable_(0)
{
}

void Chunk::Init(size_t blockSize, uint8_t numBlocks) noexcept
{
    assert(blockSize > 0);
    assert(numBlocks > 0);

    // allocate new memory
    pData_ = new uint8_t[blockSize * numBlocks];

    // Update data end pointer (const hack to be able to modify a const pointer after
    // initialization)
    uint8_t** ppDataEnd_ = const_cast<uint8_t**>(&pDataEnd_);
    *ppDataEnd_ = pData_ + (blockSize * numBlocks);

    // initialize blocks
    Reset(blockSize, numBlocks);
}

void* Chunk::Allocate(size_t blockSize) noexcept
{
    if (blocksAvailable_ == 0)
    {
        return nullptr;
    }

    uint8_t* resultPtr = pData_ + (nextAvailableBlock_ * blockSize);

    // Update nextAvailableBlock_ to point to the next block
    nextAvailableBlock_ = *resultPtr;
    --blocksAvailable_;
    return resultPtr;
}

void Chunk::Deallocate(void* pBlock, size_t blockSize, size_t numBlocks) noexcept
{
    uint8_t* pBlockToRelease = static_cast<uint8_t*>(pBlock);

    // boundary check
    assert(pBlockToRelease >= pData_);
    assert(pBlockToRelease < (pData_ + (numBlocks * blockSize)));

    // Alignment check
    assert((pBlockToRelease - pData_) % blockSize == 0);

    // update current block's "list pointer" to point to the block that was previously the first
    // available block
    *pBlockToRelease = nextAvailableBlock_;

    // set the first available block to "point" at the block that is being deallocated
    nextAvailableBlock_ = static_cast<uint8_t>((pBlockToRelease - pData_) / blockSize);

    // Truncation check
    assert(nextAvailableBlock_ == (pBlockToRelease - pData_) / blockSize);

    ++blocksAvailable_;
}

void Chunk::Reset(size_t blockSize, uint8_t numBlocks)
{
    assert(blockSize > 0);
    assert(numBlocks > 0);

    // overflow check
    assert((blockSize * numBlocks) / blockSize == numBlocks);

    nextAvailableBlock_ = 0;

    blocksAvailable_ = numBlocks;

    uint8_t* dataPtr = pData_;
    for (uint8_t i = 0; i != numBlocks; dataPtr += blockSize)
    {
        // assign first byte in each block to the index of the next block
        // ie linked list
        *dataPtr = ++i;
    }
}

void Chunk::Release() noexcept { delete[] pData_; }

bool Chunk::IsInChunk(const void* pBlock, size_t blockSize, size_t numBlocks) const noexcept
{
    auto pBlockToCheck = reinterpret_cast<const uint8_t*>(pBlock);

    // check if pBlock falls within the memory space of this Chunk
    return (pBlockToCheck >= pData_) && (pBlockToCheck < pDataEnd_);
}
