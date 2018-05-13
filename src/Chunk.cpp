#include "Chunk.h"
#include <assert.h>


Chunk::Chunk(const Chunk& rhs) {
  Init(rhs.blockSize_, rhs.blocks_);
  for (unsigned int i = 0; i < blockSize_ * blocks_; i++) {
    pData_[i] = rhs.pData_[i];
  }
}


void Chunk::Init(size_t blockSize, unsigned char blocks) {
  assert(blockSize > 0);
  assert(blocks > 0);
  blockSize_ = blockSize;
  blocks_ = blocks;
  // overflow check
  assert((blockSize * blocks) / blockSize == blocks);
  pData_ = new unsigned char[blockSize * blocks];
  Reset(blockSize, blocks);
}


void* Chunk::Allocate(size_t blockSize) {
  if (!blocksAvailable_) {
    return 0;
  }
  assert((firstAvailableBlock_ * blockSize) / blockSize == firstAvailableBlock_);
  unsigned char* resultPtr = pData_ + (firstAvailableBlock_ * blockSize);
  // Update firstAvailableBlock_ to point to the next block
  firstAvailableBlock_ = *resultPtr;
  --blocksAvailable_;
  return resultPtr;
}


void Chunk::Deallocate(void* p, size_t blockSize) {
  assert(p >= pData_);
  unsigned char* toReleasePtr = static_cast<unsigned char*>(p);
  // Alignment check
  assert((toReleasePtr - pData_) % blockSize == 0);
  *toReleasePtr = firstAvailableBlock_;
  firstAvailableBlock_ = static_cast<unsigned char>((toReleasePtr - pData_) / blockSize);
  // Truncation check
  assert(firstAvailableBlock_ == (toReleasePtr - pData_) / blockSize);
  ++blocksAvailable_;
}


void Chunk::Reset(size_t blockSize, unsigned char blocks) {
  assert(blockSize > 0);
  assert(blocks > 0);
  // overflow check
  assert((blockSize * blocks) / blockSize == blocks);
  firstAvailableBlock_ = 0;
  blocksAvailable_ = blocks;
  unsigned char* dataPtr = pData_;
  for (unsigned char i = 0; i != blocks; dataPtr += blockSize) {
    // assign first byte of each unused block to index of the next unused block
    *dataPtr = ++i;
  }
}


void Chunk::Release() {
  delete[] pData_;
}
