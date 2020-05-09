#pragma once

#include <cstdlib>
#include <vector>

#include "FixedAllocator.hpp"

class SmallObjAllocator {
 public:
  /// @brief create a SmallObjAllocator to handle allocations up to a maximum size (in bytes)
  /// @param maxObjectSize The maximum size (in bytes) of objects to allocate
  explicit SmallObjAllocator(size_t maxObjectSize) noexcept;

  ~SmallObjAllocator() noexcept = default;

  /// @brief allocate memory
  /// @param numBytes The size (in bytes) of the object to allocate memory for
  void* Allocate(size_t numBytes) noexcept;

  /// deallocate memory at pointer p
  void Deallocate(void* pObjectToDealloc, size_t numBytes) noexcept;

 private:
  using Pool = std::vector<alloc::FixedAllocator>;

  // max num of bytes handled
  size_t maxObjectSize_;

  // LRU FixedAllocator
  alloc::FixedAllocator* pLastAlloc_;

  // LRU FIxedAllocator
  alloc::FixedAllocator* pLastDealloc_;

  // holds pool of FixedAllocator objects that handle various sized requests
  // Sorted by the size of the objects each FixedAllocator object handles
  Pool pool_;
};
