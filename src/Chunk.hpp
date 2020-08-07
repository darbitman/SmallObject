#pragma once

#include <cstdint>
#include <cstdlib>
#include <memory>

namespace alloc {

class Chunk {
 public:
  Chunk() noexcept;

  Chunk(Chunk&&) noexcept = default;

  Chunk& operator=(Chunk&&) noexcept = default;

  ~Chunk() noexcept = default;

  /// @brief Initialize Chunk memory address space
  /// @param block_size The size of each object (in bytes)
  /// @param num_blocks The number of objects in this Chunk
  void Init(size_t block_size, uint8_t num_blocks) noexcept;

  /// @brief Allocate memory and return a pointer to it to the client
  /// @param block_size Size of the block to allocate memory for
  /// @return void* Returns a pointer to a block of size block_size (in bytes). Returns a nullptr if no free memory
  /// exists or if the memory has not been initialized with the Init call.
  void* Allocate(size_t block_size) noexcept;

  /// @brief Deallocate memory and add back to pool
  /// @param p_block Pointer to the block to return to memory pool
  /// @param block_size Size of each block (in bytes)
  /// @param num_blocks Total number of blocks in the memory space of this Chunk
  void Deallocate(void* p_block, size_t block_size) noexcept;

  /// @brief Return memory back to OS
  void Release() noexcept;

  /// @brief Checks if the block pointed to by the pointer falls somewhere in the memory space of this Chunk
  /// @param p_block The pointer to a block that needs to be checked
  /// @return bool Returns true if the block is in this Chunk's memory. False, otherwise.
  bool IsInChunk(const void* p_block) const noexcept;

  /// @brief Return the number of memory blocks available in this Chunk
  /// @return uint8_t
  uint8_t GetNumBlocksAvailable() const noexcept;

  /// Deleted to prevent misuse
  Chunk(const Chunk&) = delete;
  Chunk& operator=(const Chunk&) = delete;

 private:
  /// @brief Initialize the blocks in this Chunk in the form of a "linked list". Does not zero out memory
  /// @param block_size Size of each block (in bytes)
  /// @param num_blocks Total number of blocks in the memory space of this Chunk
  void Reset(size_t block_size, uint8_t num_blocks);

  /// Points to the beginning of the memory space in this Chunk
  std::unique_ptr<uint8_t[]> p_data_;

  /// Points to the block past the last one in memory
  const uint8_t* p_data_end_;

  /// index into the p_data_ array
  uint8_t next_available_block_;

  uint8_t blocks_available_;
};

}  // namespace alloc
