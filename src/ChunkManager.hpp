#pragma once

#include "CommonTypes.hpp"

namespace alloc {

class ChunkManager {
 public:
  /// @brief
  /// @param block_size The size of blocks (in bytes) allocated by managed Chunks
  explicit ChunkManager(size_t block_size) noexcept;

  size_t GetBlockSize() const noexcept;

  Chunk& FindFreeChunk() noexcept;

  /// @brief Finds the Chunk that performed a memory allocation for a block pointed to by p_block. This Chunk is one
  /// of potentially many managed by this ChunkManager
  /// @param p_block Points at a block that was allocated by a Chunk managed by this ChunkManager
  /// @param starting_chunk Should be a reference to a Chunk in this ChunkManager. This provides a good starting point
  /// if the client keeps track of LRU Chunks. If this information is not kept, then a reference to any Chunk managed by
  /// this ChunkManager can be passed in.
  /// @return Chunk* A valid Chunk that allocated the memory pointed to by p_block.
  Chunk* FindChunkOwner(void* p_block, Chunk& starting_chunk) const noexcept;

 private:
  /// The default size (in bytes) of a Chunk
  static constexpr size_t kDefaultChunkSize = 262144;

  /// The default size (in number of Chunks) for the container of Chunks
  static constexpr size_t kDefaultNumberOfChunks = 100;

  /// Size of each block (in bytes)
  const size_t block_size_;

  /// The number of blocks a Chunk of size kDefaultChunkSize can hold
  uint8_t num_blocks_;

  Chunks chunks_;
};

}  // namespace alloc
