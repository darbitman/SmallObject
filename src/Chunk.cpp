#include "Chunk.h"
#include <assert.h>


void Chunk::Init(size_t blockSize, unsigned char blocks) {
  pData_ = new unsigned char[blockSize * blocks];
  firstAvailableBlock_ = 0;
  blocksAvailable_ = blocks;
  unsigned char* dataPtr = pData_;
  for (unsigned char i = 0; i != blocks; dataPtr += blockSize) {
    // assign first byte of each unused block to index of the next unused block
    *dataPtr = ++i;
  }
}


void* Chunk::Allocate(size_t blockSize) {
  if (!blocksAvailable_) {
    return 0;
  }
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
