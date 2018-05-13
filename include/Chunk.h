#pragma once
#include <cstdlib>

class Chunk {
public:
  Chunk() {};
  void Init(size_t blockSize, unsigned char numBlocks);
  void* Allocate(size_t blockSize);
  void Deallocate(void* p, size_t blockSize);
  void Reset(size_t blockSize, unsigned char blocks);
  void Release();
  unsigned char* pData_;
  unsigned char
    firstAvailableBlock_,
    blocksAvailable_;
private:
  size_t blockSize_;
  unsigned char blocks_;
};
