#include "FixedAllocator.hpp"

#include <cassert>
#include <limits>

#include "AllocatorUtility.hpp"

namespace alloc {

FixedAllocator::FixedAllocator(size_t block_size) noexcept
    : p_lru_alloc_chunk_(nullptr), p_lru_dealloc_chunk_(nullptr), block_size_(block_size), num_blocks_(0) {
  assert(block_size_ > 0);

  auto num_blocks = kDefaultChunkSize / block_size;

  assert(num_blocks != 0);

  // Place upper bound
  if (num_blocks > std::numeric_limits<uint8_t>::max()) {
    num_blocks = std::numeric_limits<uint8_t>::max();
  }

  num_blocks_ = static_cast<uint8_t>(num_blocks);

  chunks_.reserve(kDefaultNumberOfChunks);
}

FixedAllocator::~FixedAllocator() noexcept {
  for (auto& chunk : chunks_) {
    chunk.Release();
  }
}

void* FixedAllocator::Allocate() noexcept {
  if (p_lru_alloc_chunk_ == nullptr || p_lru_alloc_chunk_->GetNumBlocksAvailable() == 0) {
    // no chunk exists or no available memory in current chunk
    // Try to find one (linear search)
    for (Chunks::iterator chunk_iter = chunks_.begin(), chunk_iter_end = chunks_.end();; ++chunk_iter) {
      // Couldn't find a Chunk with any available space, so create a new Chunk
      if (chunk_iter == chunk_iter_end) {
        // Construct new Chunk in place at the back of the vector
        auto& new_chunk = chunks_.emplace_back();

        // Initialize new Chunk
        new_chunk.Init(block_size_, num_blocks_);

        // Update Chunk cache pointers
        p_lru_alloc_chunk_   = &new_chunk;
        p_lru_dealloc_chunk_ = &new_chunk;

        // can now exit for-loop and allocate memory
        break;
      }

      // Found a Chunk with available space
      if (chunk_iter->GetNumBlocksAvailable() > 0) {
        p_lru_alloc_chunk_ = &(*chunk_iter);

        // can now exit for-loop and allocate memory
        break;
      }
    }
  }

  assert(p_lru_alloc_chunk_ != nullptr);
  assert(p_lru_alloc_chunk_->GetNumBlocksAvailable() > 0);

  return p_lru_alloc_chunk_->Allocate(block_size_);
}

void FixedAllocator::Deallocate(void* p_object) noexcept {
  assert(!chunks_.empty());
  assert(p_lru_dealloc_chunk_ != nullptr);

  // Linearly search for a Chunk to which p_object belongs to starting at p_lru_dealloc_chunk_ and search
  // outwards
  p_lru_dealloc_chunk_ = util::FindChunkOwner(p_object, *p_lru_dealloc_chunk_, chunks_);

  assert(p_lru_dealloc_chunk_ != nullptr);

  DoDeallocate(p_object);
}

size_t FixedAllocator::getBlockSize() const noexcept { return block_size_; }

void FixedAllocator::DoDeallocate(void* p_object) noexcept {
  assert(p_lru_dealloc_chunk_->IsInChunk(p_object));

  p_lru_dealloc_chunk_->Deallocate(p_object, block_size_);

  // Check if the Chunk that just freed memory, has no allocated blocks
  // Can potentially remove the empty Chunk object if another empty one exists
  if (p_lru_dealloc_chunk_->GetNumBlocksAvailable() == num_blocks_) {
    // p_lru_dealloc_chunk_ has all blocks freed
    // check if another chunk is empty in order to release a Chunk
    auto* const p_last_chunk = &chunks_.back();

    if (p_last_chunk == p_lru_dealloc_chunk_) {
      if (chunks_.size() > 1 && p_last_chunk[-1].GetNumBlocksAvailable() == num_blocks_) {
        // have more than 1 Chunk object and last 2 Chunk objets are empty
        // can delete the last one
        p_last_chunk->Release();
        chunks_.pop_back();
        p_lru_alloc_chunk_ = p_lru_dealloc_chunk_ = &chunks_.front();
      }
      return;
    }

    // p_lru_dealloc_chunk_ and p_last_chunk are not the same Chunk
    // checking if p_last_chunk is empty
    // if empty, release it, move p_lru_dealloc_chunk_ to the back since it's also empty, then change
    // pointer p_lru_alloc_chunk_ to point to p_lru_dealloc_chunk_ since it's empty and that's where free
    // blocks will be allocated from
    if (p_last_chunk->GetNumBlocksAvailable() == num_blocks_) {
      p_last_chunk->Release();
      chunks_.pop_back();
      std::swap(*p_lru_dealloc_chunk_, chunks_.back());
      p_lru_alloc_chunk_ = p_lru_dealloc_chunk_;
    }
    // if last chunk isn't empty, move empty chunk to the back
    // set p_lru_alloc_chunk_ to point to it since that's where free blocks should be allocated from
    // since it can serve the most requests without searching for a free block
    else {
      std::swap(*p_lru_dealloc_chunk_, *p_last_chunk);

      // p_lru_alloc_chunk_ and p_lru_dealloc_chunk_ now point to the same Chunk
      p_lru_alloc_chunk_ = &chunks_.back();
    }
  }
}

}  // namespace alloc
