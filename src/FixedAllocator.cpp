#include "FixedAllocator.h"
#include <assert.h>

FixedAllocator::FixedAllocator() {
  pAllocChunk_ = 0;
  pDeallocChunk_ = 0;
}


void* FixedAllocator::Allocate() {
  if (pAllocChunk_ == 0 ||
    pAllocChunk_->blocksAvailable_ == 0) {
    // no chunk exists or no available memory in current chunk
    // Try to find one (linear search)
    for (Chunks::iterator itr = chunks_.begin(); ; itr++) {
      if (itr == chunks_.end()) {
        chunks_.reserve(chunks_.size() + 1);  // increase vectory capacity by 1
        Chunk newChunk;
        newChunk.Init(blockSize_, numBlocks_);
        chunks_.push_back(newChunk);
        pAllocChunk_ = &chunks_.back();
        pDeallocChunk_ = &chunks_.back();
        break;
      }
      if (itr->blocksAvailable_ > 0) {
        pAllocChunk_ = &(*itr);
        break;
      }
    }
  }
  assert(pAllocChunk_ != 0);
  assert(pAllocChunk_->blocksAvailable_ > 0);
  return pAllocChunk_->Allocate(blockSize_);
}


void FixedAllocator::Deallocate(void* p) {
  // check most recent deallocated Chunk first
  // check if p lies between first block address and last block address
  if (p >= pDeallocChunk_->pData_ && p <= (pDeallocChunk_->pData_ + blockSize_ * numBlocks_)) {
    pDeallocChunk_->Deallocate(p, blockSize_);
    return;
  }
  // if p does not belong to recently used Deallocated chunk
  // find the correct chunk
  for (Chunks::iterator itr = chunks_.begin(); itr != chunks_.end(); itr++) {
    if (p >= itr->pData_ && p <= (itr->pData_ + blockSize_ * numBlocks_)) {
      itr->Deallocate(p, blockSize_);
      // set new recently deallocated Chunk
      pDeallocChunk_ = &(*itr);
      return;
    }
  }
}
