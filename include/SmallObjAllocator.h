#pragma once
#include <cstdlib>
#include <vector>
#include "FixedAllocator.h"


class SmallObjAllocator {
public:
  SmallObjAllocator(size_t maxObjectSize);
  void* Allocate(size_t numBytes);
  void Deallocate(void* p, size_t numBytes);
private:
  typedef std::vector<FixedAllocator> Pool;
  Pool pool_;
  size_t maxObjectSize_;
  FixedAllocator* pLastAlloc_;
  FixedAllocator* pLastDealloc_;
  //size_t chunkSize_;
};
