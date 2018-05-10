#pragma once
#include <cstdlib>
#include <vector>
#include "FixedAllocator.h"

class SmallObjAllocator {
public:
  SmallObjAllocator(size_t chunkSize, size_t maxObjectSize);
  void* Allocate(size_t numBytes);
  void Deallocate(void* p, size_t size);
private:
  std::vector<FixedAllocator> pool_;
  FixedAllocator* pLastAlloc_;
  FixedAllocator* pLastDealloc_;
};
