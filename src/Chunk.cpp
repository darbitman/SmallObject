#include "Chunk.hpp"

Chunk::Chunk() noexcept : pData_(nullptr), pDataEnd_(nullptr), nextAvailableBlock_(0), blocksAvailable_(0) {}

void Chunk::Init(size_t blockSize, uint8_t numBlocks) noexcept {
  // allocate new memory
  pData_ = new uint8_t[blockSize * numBlocks];

  // update data end pointer
  pDataEnd_ = pData_ + (blockSize * numBlocks);

  // initialize blocks
  Reset(blockSize, numBlocks);
}

void* Chunk::Allocate(size_t blockSize) noexcept {
  if (blocksAvailable_ == 0) {
    return nullptr;
  }

  uint8_t* resultPtr = pData_ + (nextAvailableBlock_ * blockSize);

  // Update nextAvailableBlock_ to point to the next block
  nextAvailableBlock_ = *resultPtr;
  --blocksAvailable_;
  return resultPtr;
}

void Chunk::Deallocate(void* pBlock, size_t blockSize) noexcept {
  uint8_t* pBlockToRelease = static_cast<uint8_t*>(pBlock);

  // update current block's "list pointer" to point to the block that was previously the first available block
  *pBlockToRelease = nextAvailableBlock_;

  // set the first available block to "point" at the block that is being deallocated
  nextAvailableBlock_ = static_cast<uint8_t>((pBlockToRelease - pData_) / blockSize);

  ++blocksAvailable_;
}

void Chunk::Reset(size_t blockSize, uint8_t numBlocks) {
  nextAvailableBlock_ = 0;

  blocksAvailable_ = numBlocks;

  uint8_t* dataPtr = pData_;
  for (uint8_t i = 0; i != numBlocks; dataPtr += blockSize) {
    // assign first byte in each block to the index of the next block (ie contiguous linked list)
    *dataPtr = ++i;
  }
}

void Chunk::Release() noexcept {
  delete[] pData_;
  pData_              = nullptr;
  pDataEnd_           = nullptr;
  nextAvailableBlock_ = 0;
  blocksAvailable_    = 0;
}

bool Chunk::IsInChunk(const void* pBlock) const noexcept {
  auto pBlockToCheck = reinterpret_cast<const uint8_t*>(pBlock);

  // check if pBlock falls within the memory space of this Chunk
  return (pBlockToCheck >= pData_) && (pBlockToCheck < pDataEnd_);
}
