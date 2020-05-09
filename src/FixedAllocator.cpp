#include "FixedAllocator.hpp"

#include <cassert>

namespace alloc {

FixedAllocator::FixedAllocator(size_t blockSize) noexcept
    : pAllocChunk_(nullptr), pDeallocChunk_(nullptr), blockSize_(blockSize), numBlocks_(0) {
  assert(blockSize > 0);

  auto numBlocks = kDefaultChunkSize / blockSize;

  assert(numBlocks != 0);

  // Place upper bound
  if (numBlocks > UINT8_MAX) {
    numBlocks = UINT8_MAX;
  }

  numBlocks_ = static_cast<uint8_t>(numBlocks);

  assert(numBlocks_ == numBlocks);

  chunks_.reserve(kDefaultVectorReservedSize);
}

FixedAllocator::~FixedAllocator() noexcept {
  for (auto& chunk : chunks_) {
    chunk.Release();
  }
}

void* FixedAllocator::Allocate() noexcept {
  if (pAllocChunk_ == nullptr || pAllocChunk_->GetNumBlocksAvailable() == 0) {
    // no chunk exists or no available memory in current chunk
    // Try to find one (linear search)
    for (Chunks::iterator chunks_iter = chunks_.begin(), chunks_end = chunks_.end();; ++chunks_iter) {
      // Couldn't find a Chunk with any available space, so create a new Chunk
      if (chunks_iter == chunks_end) {
        // Construct new Chunk in place at the back of the vector
        auto& newChunk = chunks_.emplace_back();

        // Initialize new Chunk
        newChunk.Init(blockSize_, numBlocks_);

        // Update Chunk cache pointers
        pAllocChunk_   = &newChunk;
        pDeallocChunk_ = &newChunk;
        break;
      }

      // Found a Chunk with available space
      if (chunks_iter->GetNumBlocksAvailable() > 0) {
        pAllocChunk_ = &(*chunks_iter);
        break;
      }
    }
  }

  assert(pAllocChunk_ != nullptr);
  assert(pAllocChunk_->GetNumBlocksAvailable() > 0);

  return pAllocChunk_->Allocate(blockSize_);
}

void FixedAllocator::Deallocate(void* pObject) noexcept {
  assert(!chunks_.empty());

  // Linearly search for a Chunk to which pObject belongs to starting at pDeallocChunk_ and search
  // outwards
  pDeallocChunk_ = FindChunkOwner(pObject);

  assert(pDeallocChunk_ != nullptr);

  DoDeallocate(pObject);
}

size_t FixedAllocator::getBlockSize() const noexcept { return blockSize_; }

alloc::Chunk* FixedAllocator::FindChunkOwner(void* pObject) noexcept {
  assert(!chunks_.empty());
  assert(pDeallocChunk_ != nullptr);

  alloc::Chunk* pLowChunk       = pDeallocChunk_;
  alloc::Chunk* pHighChunk      = pDeallocChunk_ + 1;
  alloc::Chunk* pLowChunkBound  = &chunks_.front();
  alloc::Chunk* pHighChunkBound = &chunks_.back() + 1;

  // special case: pDeallocChunk_ is the last Chunk in the vector
  if (pHighChunk == pHighChunkBound) {
    pHighChunk = nullptr;
  }

  // Search outward in chunks_
  for (;;) {
    if (pLowChunk != nullptr) {
      // pObject lies in the address space of a Chunk pointed to by pLowChunk
      if (pLowChunk->IsInChunk(pObject)) {
        return pLowChunk;
      }

      if (pLowChunk == pLowChunkBound) {
        pLowChunk = nullptr;
      } else {
        --pLowChunk;
      }
    }

    if (pHighChunk != nullptr) {
      // pObject lies in the address space of a Chunk pointed to by pHighChunk
      if (pHighChunk->IsInChunk(pObject)) {
        return pHighChunk;
      }
      if (++pHighChunk == pHighChunkBound) {
        pHighChunk = nullptr;
      }
    }
  }

  // should never reach here
  return nullptr;
}

void FixedAllocator::DoDeallocate(void* pObject) noexcept {
  assert(pDeallocChunk_->IsInChunk(pObject));

  pDeallocChunk_->Deallocate(pObject, blockSize_);

  // Check if the Chunk that just freed memory, has no allocated blocks
  // Can potentially remove the empty Chunk object if another empty one exists
  if (pDeallocChunk_->GetNumBlocksAvailable() == numBlocks_) {
    // pDeallocChunk_ has all blocks freed
    // check if another chunk is empty in order to release a Chunk
    alloc::Chunk* const pLastChunk = &chunks_.back();

    if (pLastChunk == pDeallocChunk_) {
      if (chunks_.size() > 1 && pLastChunk[-1].GetNumBlocksAvailable() == numBlocks_) {
        // have more than 1 Chunk object and last 2 Chunk objets are empty
        // can delete the last one
        pLastChunk->Release();
        chunks_.pop_back();
        pAllocChunk_ = pDeallocChunk_ = &chunks_.front();
      }
      return;
    }

    // pDeallocChunk and pLastChunk are not the same Chunk
    // checking if pLastChunk is empty
    // if empty, release it, move pDeallocChunk_ to the back since it's also empty, then change
    // pointer pAllocChunk to point to pDeallocChunk since it's empty and that's where free
    // blocks will be allocated from
    if (pLastChunk->GetNumBlocksAvailable() == numBlocks_) {
      pLastChunk->Release();
      chunks_.pop_back();
      std::swap(*pDeallocChunk_, chunks_.back());
      pAllocChunk_ = pDeallocChunk_;
    }
    // if last chunk isn't empty, move empty chunk to the back
    // set pAllocChunk to point to it since that's where free blocks should be allocated from
    // since it can serve the most requests without searching for a free block
    else {
      std::swap(*pDeallocChunk_, *pLastChunk);

      // pAllocChunk_ and pDeallocChunk_ now point to the same Chunk
      pAllocChunk_ = &chunks_.back();
    }
  }
}

}  // namespace alloc
