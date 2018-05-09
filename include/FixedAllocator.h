#pragma once
#include <cstdlib>
#include <vector>
#include "Chunk.h"

class FixedAllocator {
public:
private:
  size_t blockSize_;
  unsigned char numBlocks_;
  typedef std::vector<Chunk> Chunks;
  Chunks chunks_;
  Chunk* pAllocChunk_;
  Chunk* pDeallocChunk_;
};
