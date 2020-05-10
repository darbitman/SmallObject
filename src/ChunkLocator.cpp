#include "ChunkLocator.hpp"

#include <cassert>

namespace alloc::util {

Chunk* FindChunkOwner(void* p_object, Chunk& starting_chunk, const Chunks& chunk_container) noexcept {
  assert(!chunk_container.empty());

  auto* p_lo_chunk       = &starting_chunk;
  auto* p_hi_chunk       = &starting_chunk + 1;
  auto* p_first_chunk    = &chunk_container.front();
  auto* p_hi_chunk_bound = &chunk_container.back() + 1;  // past-the-end chunk (ie. an invalid Chunk)

  // special case: starting_chunk is the last Chunk in the container
  if (p_hi_chunk == p_hi_chunk_bound) {
    p_hi_chunk = nullptr;
  }

  // search outwards from both, the starting_chunk and the one to its right
  for (;;) {
    // search to the 'left' in the container
    if (p_lo_chunk != nullptr) {
      // p_object lies in the managed memory space of the Chunk pointed to by p_lo_chunk
      if (p_lo_chunk->IsInChunk(p_object)) {
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
      // p_object lies in the address space of a Chunk pointed to by p_hi_chunk
      if (p_hi_chunk->IsInChunk(p_object)) {
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

}  // namespace alloc::util
