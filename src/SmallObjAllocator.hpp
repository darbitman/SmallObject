#pragma once

#include "FixedAllocator.hpp"

#include <cstdlib>
#include <vector>

class SmallObjAllocator
{
  public:
    // create a SmallObjAllocator to handle allocations less than maxObjectSize
    SmallObjAllocator(size_t maxObjectSize);

    // allocate memory to handle numBytes
    void* Allocate(size_t numBytes);

    // deallocate memory at pointer p
    void Deallocate(void* p, size_t numBytes);

  private:
    // holds pool of FixedAllocator objects that handle various sized requests
    typedef std::vector<FixedAllocator> Pool;
    Pool pool_;

    // max num of bytes handled
    size_t maxObjectSize_;

    // LRU FixedAllocator
    FixedAllocator* pLastAlloc_;

    // LRU FIxedAllocator
    FixedAllocator* pLastDealloc_;
};
