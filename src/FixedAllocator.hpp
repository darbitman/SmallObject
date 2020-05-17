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

  FixedAllocator(FixedAllocator&&) noexcept = default;

  FixedAllocator& operator=(FixedAllocator&&) noexcept = default;

  ~FixedAllocator() noexcept = default;

  /// @brief Allocate memory and return pointer to the block of memory
  /// @return void* Pointer to the block of memory allocated
  void* Allocate() noexcept;

  /// @brief Return memory back to Chunk handler
  /// @param p_object
  void Deallocate(void* p_object) noexcept;

  /// @brief Return the size (in bytes) of the objects that this FixedAllocator allocates
  size_t GetBlockSize() const noexcept;

  /// Deleted to prevent misuse
  FixedAllocator(const FixedAllocator&) noexcept = delete;
  FixedAllocator& operator=(const FixedAllocator&) noexcept = delete;

 private:
  /// LRU chunk that handled an allococation
  Chunk* p_lru_alloc_chunk_;

  /// LRU chunk that handled a deallocation
  Chunk* p_lru_dealloc_chunk_;

  // holds all chunks that handle block_size_ requests
  ChunkManager chunk_manager_;
};

}  // namespace alloc
