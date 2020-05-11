#include "ChunkManager.hpp"

#include <cassert>
#include <limits>

namespace alloc {

ChunkManager::ChunkManager(size_t block_size) noexcept : block_size_(block_size) {
  assert(block_size_ > 0);

  auto num_blocks = kDefaultChunkSize / block_size_;

  assert(num_blocks > 0);

  // clamp number of blocks in each Chunk
  if (num_blocks > std::numeric_limits<uint8_t>::max()) {
    num_blocks = std::numeric_limits<uint8_t>::max();
  }

  num_blocks_ = static_cast<uint8_t>(num_blocks);

  chunks_.reserve(kDefaultNumberOfChunks);
}

size_t ChunkManager::GetBlockSize() const noexcept { return block_size_; }

uint8_t ChunkManager::GetNumBlocks() const noexcept { return num_blocks_; }

Chunk& ChunkManager::FindFreeChunk() noexcept {
  for (auto& chunk : chunks_) {
    // if a Chunk with a free block was found, return it
    if (chunk.GetNumBlocksAvailable() > 0) {
      // if the found Chunk with an available block is not at the front of the container and has more than 1 block
      // available, move it to the front of the container so the next allocation is faster
      if (&chunk != &chunks_.front() && chunk.GetNumBlocksAvailable() > 1) {
        std::swap(chunk, chunks_.front());

        // the Chunk that was found and swapped to the front of the container is now at the front
        return chunks_.front();
      }

      // the one currently found is either already at the front and has an available block, or it has only 1 available
      // block
      return chunk;
    }
  }

  // a Chunk with a free block was not found, so create a new Chunk
  auto& chunk = chunks_.emplace_back();

  // initialize it
  chunk.Init(block_size_, num_blocks_);

  // return the new one to the client
  return chunk;
}

Chunk* ChunkManager::FindChunkOwner(void* p_block, Chunk& starting_chunk) const noexcept {
  assert(!chunks_.empty());

  auto p_lo_chunk       = &starting_chunk;
  auto p_hi_chunk       = &starting_chunk + 1;
  auto p_first_chunk    = &chunks_.front();
  auto p_hi_chunk_bound = &chunks_.back() + 1;  // past-the-end chunk (ie. an invalid Chunk)

  // special case: starting_chunk is the last Chunk in the container
  if (p_hi_chunk == p_hi_chunk_bound) {
    p_hi_chunk = nullptr;
  }

  // search outwards from both, the starting_chunk and the one to its right
  for (;;) {
    // search to the 'left' in the container
    if (p_lo_chunk != nullptr) {
      // p_block lies in the managed memory space of the Chunk pointed to by p_lo_chunk
      if (p_lo_chunk->IsInChunk(p_block)) {
        return p_lo_chunk;
      }

      // if the beginning has been reached, stop the 'leftward' search
      // otherwise, change pointer to point to the Chunk to the 'left'
      if (p_lo_chunk == p_first_chunk) {
        p_lo_chunk = nullptr;
      } else {
        --p_lo_chunk;
      }
    }

    // serach to the 'right' in the container
    if (p_hi_chunk != nullptr) {
      // p_block lies in the address space of a Chunk pointed to by p_hi_chunk
      if (p_hi_chunk->IsInChunk(p_block)) {
        return p_hi_chunk;
      }

      // if the end has been reached, stop the 'rightward' search
      // need to increment first, then check because next for-loop iteration could point to the past-the-end chunk which
      // is invalid. this would occur if the increment was done in an else statement
      if (++p_hi_chunk == p_hi_chunk_bound) {
        p_hi_chunk = nullptr;
      }
    }
  }

  // should never reach here because a Chunk owner MUST exist since that is the only way for a pointer to a Chunk to
  // have been given out (ie an allocation must have happened)
  assert(false);
  return nullptr;
}

}  // namespace alloc
