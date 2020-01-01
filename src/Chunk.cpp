#include "Chunk.hpp"

#include <cassert>

void Chunk::Init(size_t blockSize, uint8_t numBlocks)
{
    assert(blockSize > 0);
    assert(numBlocks > 0);
    blockSize_ = blockSize;
    blocks_ = numBlocks;
    // overflow check
    assert((blockSize * numBlocks) / blockSize == numBlocks);

    // allocate new memory
    pData_ = new uint8_t[blockSize * numBlocks];

    // initialize blocks
    Reset(blockSize, numBlocks);
}

void* Chunk::Allocate(size_t blockSize)
{
    if (!blocksAvailable_)
    {
        return nullptr;
    }

    assert((firstAvailableBlock_ * blockSize) / blockSize == firstAvailableBlock_);
    uint8_t* resultPtr = pData_ + (firstAvailableBlock_ * blockSize);
    // Update firstAvailableBlock_ to point to the next block
    firstAvailableBlock_ = *resultPtr;
    --blocksAvailable_;
    return resultPtr;
}

void Chunk::Deallocate(void* p, size_t blockSize)
{
    uint8_t* pBlockToRelease = static_cast<uint8_t*>(p);

    // boundary check
    assert(pBlockToRelease >= pData_);
    assert(pBlockToRelease < (pData_ + (blocks_ * blockSize)));

    // Alignment check
    assert((pBlockToRelease - pData_) % blockSize == 0);

    // update current block's "list pointer" to point to the block that was previously the first
    // available block
    *pBlockToRelease = firstAvailableBlock_;

    // set the first available block to "point" at the block that is being deallocated
    firstAvailableBlock_ = static_cast<uint8_t>((pBlockToRelease - pData_) / blockSize);

    // Truncation check
    assert(firstAvailableBlock_ == (pBlockToRelease - pData_) / blockSize);

    ++blocksAvailable_;
}

void Chunk::Reset(size_t blockSize, uint8_t numBlocks)
{
    assert(blockSize > 0);
    assert(numBlocks > 0);

    // overflow check
    assert((blockSize * numBlocks) / blockSize == numBlocks);

    firstAvailableBlock_ = 0;

    blocksAvailable_ = numBlocks;

    uint8_t* dataPtr = pData_;
    for (uint8_t i = 0; i != numBlocks; dataPtr += blockSize)
    {
        // assign first byte of each unused block to index of the next unused block
        *dataPtr = ++i;
    }
}

void Chunk::Release() { delete[] pData_; }

uint8_t Chunk::getNumBlocks() const { return blocks_; }
