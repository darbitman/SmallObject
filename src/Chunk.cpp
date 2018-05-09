#include "Chunk.h"


void Chunk::Init(size_t blockSize, unsigned char blocks) {
  pData_ = new unsigned char[blockSize * blocks];
  firstAvailableBlock_ = 0;
  blocksAvailable_ = blocks;
  unsigned char* dataPtr = pData_;
  for (unsigned char i = 0; i != blocks; dataPtr += blockSize) {
    *dataPtr = ++i;                               // assign first byte of each unused block to index of the next unused block
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
}