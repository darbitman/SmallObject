#include "SmallObjAllocator.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace alloc {

namespace {

bool CompareFixedAllocatorSize(const FixedAllocator& FixedAllocatorObj, size_t max_object_size) {
  return FixedAllocatorObj.getBlockSize() < max_object_size;
}

}  // namespace

SmallObjAllocator::SmallObjAllocator(size_t max_object_size) noexcept
    : max_object_size_(max_object_size), p_lru_allocator_(nullptr), p_lru_deallocator_(nullptr) {}

void* SmallObjAllocator::Allocate(size_t obj_size) noexcept {
  if (obj_size > max_object_size_) {
    // if obj_size isn't handle by this SmallObjAllocator, use global
    return ::operator new(obj_size);
  }

  /*
    // check if recently used FixedAllocator object services obj_size
    // if it doesn't, search pool_ for one
    // if one doesn't exist in pool_, create a new one
    if (p_lru_allocator_ != nullptr && p_lru_allocator_->getBlockSize() == obj_size) {
      return p_lru_allocator_->Allocate();
    }

    // check current pool for FixedAllocator that services at least obj_size.
    // create a new FixedAllocator if one is not found append one that handles obj_size
    Pool::iterator iter = std::lower_bound(pool_.begin(), pool_.end(), obj_size, CompareFixedAllocatorSize);

    // lower_bound may have found an object that handles allocations for objects of AT LEAST size
    // obj_size. Check to make sure a FixedAllocator object exists that handles objects of size
    // obj_size
    if (iter == pool_.end() || iter->getBlockSize() != obj_size) {
      iter               = pool_.insert(iter, FixedAllocator(obj_size));
      p_lru_deallocator_ = &(pool_.back());
    }
    p_lru_allocator_ = &(*iter);
    */
  p_lru_allocator_ = FindAllocator(obj_size);
  return p_lru_allocator_->Allocate();
}

void SmallObjAllocator::Deallocate(void* p_object_to_dealloc, size_t obj_size) noexcept {
  if (obj_size > max_object_size_) {
    ::operator delete(p_object_to_dealloc);
    return;
  }

  /*
    // check if recently used FixedAllocator object services obj_size
    // if it doesn't, search pool_ for one
    if (p_lru_deallocator_ && p_lru_deallocator_->getBlockSize() == obj_size) {
      p_lru_deallocator_->Deallocate(p_object_to_dealloc);
    }

    // check current pool for FixedAllocator that services obj_size
    Pool::iterator iter = std::lower_bound(pool_.begin(), pool_.end(), obj_size, CompareFixedAllocatorSize);

    // assert instead of an if because the if is in the Allocate method
    // It wouldn't make sense to create a new FixedAllocator object to handle a deallocation when it
    // didn't handle the allocation
    assert(iter != pool_.end());
    assert(iter->getBlockSize() == obj_size);

    p_lru_deallocator_ = &(*iter);
    */
  p_lru_deallocator_ = FindDeallocator(p_object_to_dealloc, obj_size);
  p_lru_deallocator_->Deallocate(p_object_to_dealloc);
}

FixedAllocator* SmallObjAllocator::FindAllocator(size_t obj_size) noexcept {
  // check if recently used FixedAllocator object services obj_size
  // if it doesn't, search pool_ for one
  // if one doesn't exist in pool_, create a new one
  if (p_lru_allocator_ != nullptr && p_lru_allocator_->getBlockSize() == obj_size) {
    return p_lru_allocator_;
  }

  // check current pool for FixedAllocator that services objects of at least obj_size
  // create a new FixedAllocator if one is not found append one that handles obj_size
  const auto allocator = std::lower_bound(pool_.begin(), pool_.end(), obj_size, CompareFixedAllocatorSize);
  if (allocator == pool_.end() || allocator->getBlockSize() != obj_size) {
    const auto new_allocator = pool_.insert(allocator, FixedAllocator(obj_size));

    // update since old pointer may be invalid due to container changing size
    p_lru_deallocator_ = &(pool_.back());

    return &(*new_allocator);
  } else {
    return &(*allocator);
  }
}

FixedAllocator* SmallObjAllocator::FindDeallocator(void* p_object, size_t obj_size) noexcept {
  // check if recently used FixedAllocator object services obj_size
  // if it doesn't, search pool_ for one
  if (p_lru_deallocator_ && p_lru_deallocator_->getBlockSize() == obj_size) {
    p_lru_deallocator_->Deallocate(p_object);
  }

  // check current pool for FixedAllocator that services obj_size
  const auto deallocator = std::lower_bound(pool_.begin(), pool_.end(), obj_size, CompareFixedAllocatorSize);

  // guaranteed that a FixedAllocator object is found, because an object had to have existed to handle the allocation

  return &(*deallocator);
}

}  // namespace alloc
