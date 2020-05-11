#pragma once

#include <cstdlib>
#include <vector>

#include "ChunkManager.hpp"
#include "CommonTypes.hpp"

namespace alloc {

class FixedAllocator {
 public:
  /// @brief Create a FixedAlloctor object to handle allocating memory to objects of block_size bytes
  /// @param block_size Size of objects (in bytes)
  explicit FixedAllocator(size_t block_size) noexcept;

  /// @brief
  FixedAllocator(FixedAllocator&&) noexcept = default;

  /// @brief
  FixedAllocator& operator=(FixedAllocator&&) noexcept = default;

  /// @brief
  ~FixedAllocator() noexcept = default;

  /// @brief Allocate memory and return pointer to the block of memory
  void* Allocate() noexcept;

  /// @brief Return memory back to Chunk handler
  /// @param p_object
  void Deallocate(void* p_object) noexcept;

  /// @brief Return the size (in bytes) of the objects that this FixedAllocator allocates
  size_t getBlockSize() const noexcept;

  /// Deleted to prevent misuse
  FixedAllocator(const FixedAllocator&) noexcept = delete;
  FixedAllocator& operator=(const FixedAllocator&) noexcept = delete;

 private:
  /// @brief Perform the deallocation of block pointed to by p_object. The block must lie in the memory space of the
  /// Chunk pointed to by p_lru_dealloc_chunk_
  /// @param p_object Points at the block whose memory to free
  void DoDeallocate(void* p_object) noexcept;

  /// LRU chunk that handled an allococation
  Chunk* p_lru_alloc_chunk_;

  /// LRU chunk that handled a deallocation
  Chunk* p_lru_dealloc_chunk_;

  // holds all chunks that handle block_size_ requests
  ChunkManager chunk_manager_;
};

}  // namespace alloc
