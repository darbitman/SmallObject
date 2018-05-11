#pragma once
#include <cstdlib>
#include <vector>
#include "Chunk.h"

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 4096
#endif

class FixedAllocator {
public:
  explicit FixedAllocator(size_t blockSize = 0);
  ~FixedAllocator();
  void* Allocate();
  void Deallocate(void* p);
  size_t getBlockSize() const;
private:
  size_t blockSize_;
  unsigned char numBlocks_;
  typedef std::vector<Chunk> Chunks;
  Chunks chunks_;
  Chunk* pAllocChunk_;
  Chunk* pDeallocChunk_;
  Chunk* findNearby(void* p);
  void DoDeallocate(void* p);
};
