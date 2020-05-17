#include "SmallObjAllocator.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace alloc {

namespace {

bool CompareFixedAllocatorSize(const FixedAllocator& FixedAllocatorObj, size_t max_object_size) {
  return FixedAllocatorObj.GetBlockSize() < max_object_size;
}

}  // namespace

SmallObjAllocator::SmallObjAllocator(size_t max_object_size) noexcept
    : max_object_size_(max_object_size), p_lru_allocator_(nullptr), p_lru_deallocator_(nullptr) {}

void* SmallObjAllocator::Allocate(size_t obj_size) noexcept {
  if (obj_size > max_object_size_) {
    // if obj_size isn't handle by this SmallObjAllocator, use global
    return ::operator new(obj_size);
  }

  p_lru_allocator_ = FindAllocator(obj_size);
  return p_lru_allocator_->Allocate();
}

void SmallObjAllocator::Deallocate(void* p_object_to_dealloc, size_t obj_size) noexcept {
  if (obj_size > max_object_size_) {
    ::operator delete(p_object_to_dealloc);
    return;
  }

  p_lru_deallocator_ = FindDeallocator(p_object_to_dealloc, obj_size);
  p_lru_deallocator_->Deallocate(p_object_to_dealloc);
}

FixedAllocator* SmallObjAllocator::FindAllocator(size_t obj_size) noexcept {
  // check if recently used FixedAllocator object services obj_size
  // if it doesn't, search pool_ for one
  // if one doesn't exist in pool_, create a new one
  if (p_lru_allocator_ != nullptr && p_lru_allocator_->GetBlockSize() == obj_size) {
    return p_lru_allocator_;
  }

  // check current pool for FixedAllocator that services objects of at least obj_size
  // create a new FixedAllocator if one is not found append one that handles obj_size
  const auto allocator = std::lower_bound(pool_.begin(), pool_.end(), obj_size, CompareFixedAllocatorSize);
  if (allocator == pool_.end() || allocator->GetBlockSize() != obj_size) {
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
  if (p_lru_deallocator_ && p_lru_deallocator_->GetBlockSize() == obj_size) {
    p_lru_deallocator_->Deallocate(p_object);
  }

  // check current pool for FixedAllocator that services obj_size
  const auto deallocator = std::lower_bound(pool_.begin(), pool_.end(), obj_size, CompareFixedAllocatorSize);

  // guaranteed that a FixedAllocator object is found, because an object had to have existed to handle the allocation
  return &(*deallocator);
}

}  // namespace alloc
