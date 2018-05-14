#include "SmallObjAllocator.h"
#include <assert.h>
#include <algorithm>


SmallObjAllocator::SmallObjAllocator(size_t chunkSize, size_t maxObjectSIze):
  chunkSize_(chunkSize), maxObjectSize_(maxObjectSIze),
  pLastAlloc_(0), pLastDealloc_(0) {
}


void* SmallObjAllocator::Allocate(size_t numBytes) {
  if (numBytes > maxObjectSize_) {
    return operator new(numBytes);
  }
  // check if recently used FixedAllocator object services numBytes
  // if it doesn't, search pool_ for one
  // if one doesn't exist in pool_, create a new one
  if (pLastAlloc_ && pLastAlloc_->getBlockSize() == numBytes) {
    return pLastAlloc_->Allocate();
  }
  // check current pool for FixedAllocator that services numBytes
  // if one is not found append one that handles numBytes
  Pool::iterator itr = std::lower_bound(pool_.begin(), pool_.end(), numBytes, CompareFixedAllocatorSize);
  if (itr == pool_.end() || itr->getBlockSize() != numBytes) {
    itr = pool_.insert(itr, FixedAllocator(numBytes));
    pLastDealloc_ = &(pool_.back());
  }
  pLastAlloc_ = &(*itr);
  return pLastAlloc_->Allocate();
}


void SmallObjAllocator::Deallocate(void* p, size_t numBytes) {
  if (numBytes > maxObjectSize_) {
    return operator delete(p);
  }
  // check if recently used FixedAllocator object services numBytes
  // if it doesn't, search pool_ for one
  if (pLastDealloc_ && pLastDealloc_->getBlockSize() == numBytes) {
    pLastDealloc_->Deallocate(p);
  }
  // check current pool for FixedAllocator that services numBytes
  Pool::iterator itr = std::lower_bound(pool_.begin(), pool_.end(), numBytes, CompareFixedAllocatorSize);
  assert(itr != pool_.end());
  assert(itr->getBlockSize() == numBytes);
  pLastDealloc_ = &(*itr);
  pLastDealloc_->Deallocate(p);
}

namespace {
  bool CompareFixedAllocatorSize(const FixedAllocator& FixedAllocatorObj, size_t maxObjectSize) {
    return FixedAllocatorObj.getBlockSize() < maxObjectSize;
  }
}