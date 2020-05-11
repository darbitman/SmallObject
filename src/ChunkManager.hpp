#pragma once

#include "CommonTypes.hpp"

namespace alloc {

class ChunkManager {
 public:
  /// @brief Construct an object that will manage Chunks that allocate blocks of size block_size (in bytes)
  /// @param block_size The size of blocks (in bytes) allocated by managed Chunks
  explicit ChunkManager(size_t block_size) noexcept;

  ChunkManager(ChunkManager&&) noexcept = default;

  ChunkManager& operator=(ChunkManager&&) noexcept = default;

  /// @brief Release all Chunk objects to OS
  ~ChunkManager() noexcept;

  /// @brief Get the block size (in bytes) that managed Chunks allocate
  /// @return size_t Block size (in bytes)
  size_t GetBlockSize() const noexcept;

  /// @brief Get the number of available blocks per Chunk that hasn't allocated any blocks
  /// @return uint8_t
  uint8_t GetNumBlocks() const noexcept;

  /// @brief Search for a Chunk with an available block. If one is not found, a new Chunk is created. It is possible for
  /// all of the client's references and pointers to Chunk objects to be invalidated after this method returns.
  /// @return Chunk& A Chunk object that has at least one free block to allocate
  Chunk& FindFreeChunk() noexcept;

  /// @brief Creates a new Chunk object. It is possible for all of the client's references and pointers to Chunk objects
  /// to be invalidated after this method returns.
  /// @return Chunk& Reference to the newly created Chunk object
  Chunk& CreateNewChunk() noexcept;

  /// @brief Finds the Chunk that performed a memory allocation for a block pointed to by p_block. This Chunk is one
  /// of potentially many managed by this ChunkManager
  /// @param p_block Points at a block that was allocated by a Chunk managed by this ChunkManager
  /// @param starting_chunk Should be a reference to a Chunk in this ChunkManager. This provides a good starting point
  /// if the client keeps track of LRU Chunks. If this information is not kept, then a reference to any Chunk managed by
  /// this ChunkManager can be passed in.
  /// @return Chunk* A valid Chunk that allocated the memory pointed to by p_block.
  Chunk* FindChunkOwner(void* p_block, Chunk& starting_chunk) const noexcept;

  /// Deleted to prevent misuse
  ChunkManager(const ChunkManager&) = delete;
  ChunkManager& operator=(const ChunkManager&) = delete;

 private:
  /// The default size (in bytes) of a Chunk
  static constexpr size_t kDefaultChunkSize = 262144;

  /// The default size (in number of Chunks) for the container of Chunks
  static constexpr size_t kDefaultNumberOfChunks = 100;

  uint8_t ComputeNumBlocks(size_t block_size) const noexcept;

  Chunks chunks_;

  /// Size of each block (in bytes)
  size_t block_size_;

  /// The number of blocks a Chunk of size kDefaultChunkSize can hold
  uint8_t num_blocks_;
};

}  // namespace alloc
