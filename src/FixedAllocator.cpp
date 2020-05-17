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
  // assume that p_lru_alloc_chunk_ is not a nullptr because it would've been assigned a pointer to a valid object after
  // the first alloocation
  // assume that p_lru_alloc_chunk_ pointed to the Chunk that allocated it
  p_lru_dealloc_chunk_ = chunk_manager_.FindChunkOwner(p_object, *p_lru_alloc_chunk_);

  assert(p_lru_dealloc_chunk_ != nullptr);
  assert(p_lru_dealloc_chunk_->IsInChunk(p_object));

  p_lru_dealloc_chunk_->Deallocate(p_object, chunk_manager_.GetBlockSize());
}

size_t FixedAllocator::GetBlockSize() const noexcept { return chunk_manager_.GetBlockSize(); }

}  // namespace alloc
