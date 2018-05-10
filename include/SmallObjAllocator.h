#pragma once
#include <cstdlib>

class SmallObjAllocator {
public:
  SmallObjAllocator(size_t chunkSize, size_t maxObjectSize);
  void* Allocate(size_t numBytes);
  void Deallocate(void* p, size_t size);
private:
};
