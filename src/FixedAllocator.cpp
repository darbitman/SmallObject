#include "FixedAllocator.hpp"

#include <cassert>
#include <limits>

#include "AllocatorUtility.hpp"
#include "BranchOptimization.hpp"

namespace alloc {

FixedAllocator::FixedAllocator(size_t block_size) noexcept
    : p_lru_alloc_chunk_(nullptr), p_lru_dealloc_chunk_(nullptr), chunk_manager_(block_size) {}

void* FixedAllocator::Allocate() noexcept {
  if (UNLIKELY(p_lru_alloc_chunk_ == nullptr || p_lru_alloc_chunk_->GetNumBlocksAvailable() == 0)) {
    // // no chunk exists or no available memory in current chunk
    // // Try to find one (linear search)
    // for (Chunks::iterator chunk_iter = chunks_.begin(), chunk_iter_end = chunks_.end();; ++chunk_iter) {
    //   // Couldn't find a Chunk with any available space, so create a new Chunk
    //   if (chunk_iter == chunk_iter_end) {
    //     // Construct new Chunk in place at the back of the vector
    //     auto& new_chunk = chunks_.emplace_back();

    //     // Initialize new Chunk
    //     new_chunk.Init(block_size_, num_blocks_);

    //     // Update Chunk cache pointers
    //     p_lru_alloc_chunk_   = &new_chunk;
    //     p_lru_dealloc_chunk_ = &new_chunk;

    //     // can now exit for-loop and allocate memory
    //     break;
    //   }

    //   // Found a Chunk with available space
    //   if (chunk_iter->GetNumBlocksAvailable() > 0) {
    //     p_lru_alloc_chunk_ = &(*chunk_iter);

    //     // can now exit for-loop and allocate memory
    //     break;
    //   }
    // }

    p_lru_alloc_chunk_   = &chunk_manager_.FindFreeChunk();
    p_lru_dealloc_chunk_ = p_lru_alloc_chunk_;
  }

  assert(p_lru_alloc_chunk_ != nullptr);
  assert(p_lru_alloc_chunk_->GetNumBlocksAvailable() > 0);

  return p_lru_alloc_chunk_->Allocate(chunk_manager_.GetBlockSize());
}

void FixedAllocator::Deallocate(void* p_object) noexcept {
  assert(p_lru_alloc_chunk_ != nullptr);

  // Linearly search for a Chunk to which p_object belongs to
  // assume that p_lru_alloc_chunk_ pointed to the Chunk that allocated it
  p_lru_dealloc_chunk_ = chunk_manager_.FindChunkOwner(p_object, *p_lru_alloc_chunk_);

  assert(p_lru_dealloc_chunk_ != nullptr);

  DoDeallocate(p_object);
}

size_t FixedAllocator::getBlockSize() const noexcept { return chunk_manager_.GetBlockSize(); }

void FixedAllocator::DoDeallocate(void* p_object) noexcept {
  assert(p_lru_dealloc_chunk_->IsInChunk(p_object));

  p_lru_dealloc_chunk_->Deallocate(p_object, chunk_manager_.GetBlockSize());

  // // Check if the Chunk that just freed memory, has no allocated blocks
  // // Can potentially remove the empty Chunk object if another empty one exists
  // if (p_lru_dealloc_chunk_->GetNumBlocksAvailable() == chunk_manager_.GetNumBlocks()) {
  //   // p_lru_dealloc_chunk_ has all blocks freed
  //   // check if another chunk is empty in order to release a Chunk
  //   auto* const p_last_chunk = &chunks_.back();

  //   if (p_last_chunk == p_lru_dealloc_chunk_) {
  //     if (chunks_.size() > 1 && p_last_chunk[-1].GetNumBlocksAvailable() == chunk_manager_.GetNumBlocks()) {
  //       // have more than 1 Chunk object and last 2 Chunk objets are empty
  //       // can delete the last one
  //       p_last_chunk->Release();
  //       chunks_.pop_back();
  //       p_lru_alloc_chunk_ = p_lru_dealloc_chunk_ = &chunks_.front();
  //     }
  //     return;
  //   }

  //   // p_lru_dealloc_chunk_ and p_last_chunk are not the same Chunk
  //   // checking if p_last_chunk is empty
  //   // if empty, release it, move p_lru_dealloc_chunk_ to the back since it's also empty, then change
  //   // pointer p_lru_alloc_chunk_ to point to p_lru_dealloc_chunk_ since it's empty and that's where free
  //   // blocks will be allocated from
  //   if (p_last_chunk->GetNumBlocksAvailable() == num_blocks_) {
  //     p_last_chunk->Release();
  //     chunks_.pop_back();
  //     std::swap(*p_lru_dealloc_chunk_, chunks_.back());
  //     p_lru_alloc_chunk_ = p_lru_dealloc_chunk_;
  //   }
  //   // if last chunk isn't empty, move empty chunk to the back
  //   // set p_lru_alloc_chunk_ to point to it since that's where free blocks should be allocated from
  //   // since it can serve the most requests without searching for a free block
  //   else {
  //     std::swap(*p_lru_dealloc_chunk_, *p_last_chunk);

  //     // p_lru_alloc_chunk_ and p_lru_dealloc_chunk_ now point to the same Chunk
  //     p_lru_alloc_chunk_ = &chunks_.back();
  //   }
  // }
}

}  // namespace alloc
