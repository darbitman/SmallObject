#pragma once

#include <cstdlib>
#include <vector>

#include "FixedAllocator.hpp"

namespace alloc {

class SmallObjAllocator {
 public:
  /// @brief create a SmallObjAllocator to handle allocations up to a maximum size (in bytes)
  /// @param max_object_size Largest object size that this SmallObjAllocator allocates memory for (in bytes)
  explicit SmallObjAllocator(size_t max_object_size) noexcept;

  /// @brief destructor Cleans up memory
  ~SmallObjAllocator() noexcept = default;

  /// @brief allocate memory
  /// @param obj_size Size of the object to allocate memory for (in bytes)
  /// @return void* Points to a memory block of size obj_size
  void* Allocate(size_t obj_size) noexcept;

  /// @brief deallocate memory pointed to by p_object_to_dealloc
  /// @param p_object_to_dealloc Points to the object whose memory to deallocate
  /// @param obj_size Size of the object (in bytes)
  void Deallocate(void* p_object_to_dealloc, size_t obj_size) noexcept;

 private:
  using Pool = std::vector<FixedAllocator>;

  /// @brief Find a FixedAllocator object that handles memory allocations for objects of size obj_size. If a new
  /// FixedAllocator was created to allocate objects of size obj_size, then p_lru_deallocator_ gets updated since pool_
  /// may have been modified.
  /// @param obj_size Size of the object (in bytes)
  /// @return FixedAllocator* Pointer to a FixedAllocator that handles allocating memory for objects of size obj_size
  FixedAllocator* FindAllocator(size_t obj_size) noexcept;

  FixedAllocator* FindDeallocator(void* p_object, size_t obj_size) noexcept;

  // max num of bytes handled
  const size_t max_object_size_;

  // holds pool of FixedAllocator objects that handle various sized requests
  // Sorted by the size of the objects each FixedAllocator object handles
  Pool pool_;

  // LRU FixedAllocator
  FixedAllocator* p_lru_allocator_;

  // LRU FIxedAllocator
  FixedAllocator* p_lru_deallocator_;
};

}  // namespace alloc
