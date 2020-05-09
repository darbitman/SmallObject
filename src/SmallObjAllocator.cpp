#include "SmallObjAllocator.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace {
bool CompareFixedAllocatorSize(const FixedAllocator& FixedAllocatorObj, size_t maxObjectSize) {
  return FixedAllocatorObj.getBlockSize() < maxObjectSize;
}
}  // namespace

SmallObjAllocator::SmallObjAllocator(size_t maxObjectSize) noexcept
    : maxObjectSize_(maxObjectSize), pLastAlloc_(nullptr), pLastDealloc_(nullptr) {}

void* SmallObjAllocator::Allocate(size_t numBytes) noexcept {
  if (numBytes > maxObjectSize_) {
    return operator new(numBytes);
  }

  // check if recently used FixedAllocator object services numBytes
  // if it doesn't, search pool_ for one
  // if one doesn't exist in pool_, create a new one
  if (pLastAlloc_ != nullptr && pLastAlloc_->getBlockSize() == numBytes) {
    return pLastAlloc_->Allocate();
  }

  // check current pool for FixedAllocator that services at least numBytes.
  // create a new FixedAllocator if one is not found append one that handles numBytes
  Pool::iterator iter = std::lower_bound(pool_.begin(), pool_.end(), numBytes, CompareFixedAllocatorSize);

  // lower_bound may have found an object that handles allocations for objects of AT LEAST size
  // numBytes. Check to make sure a FixedAllocator object exists that handles objects of size
  // numBytes
  if (iter == pool_.end() || iter->getBlockSize() != numBytes) {
    iter          = pool_.insert(iter, FixedAllocator(numBytes));
    pLastDealloc_ = &(pool_.back());
  }
  pLastAlloc_ = &(*iter);
  return pLastAlloc_->Allocate();
}

void SmallObjAllocator::Deallocate(void* pObjectToDealloc, size_t numBytes) noexcept {
  if (numBytes > maxObjectSize_) {
    operator delete(pObjectToDealloc);
    return;
  }

  // check if recently used FixedAllocator object services numBytes
  // if it doesn't, search pool_ for one
  if (pLastDealloc_ && pLastDealloc_->getBlockSize() == numBytes) {
    pLastDealloc_->Deallocate(pObjectToDealloc);
  }

  // check current pool for FixedAllocator that services numBytes
  Pool::iterator iter = std::lower_bound(pool_.begin(), pool_.end(), numBytes, CompareFixedAllocatorSize);

  // assert instead of an if because the if is in the Allocate method
  // It wouldn't make sense to create a new FixedAllocator object to handle a deallocation when it
  // didn't handle the allocation
  assert(iter != pool_.end());
  assert(iter->getBlockSize() == numBytes);

  pLastDealloc_ = &(*iter);
  pLastDealloc_->Deallocate(pObjectToDealloc);
}
