#include "FixedAllocator.h"
#include <assert.h>

FixedAllocator::FixedAllocator(size_t blockSize) : 
  blockSize_(blockSize), pAllocChunk_(0), pDeallocChunk_(0) {
    assert(blockSize > 0);
    size_t numBlocks = DEFAULT_CHUNK_SIZE / blockSize;
    if (numBlocks > UCHAR_MAX) {
      numBlocks = UCHAR_MAX;
    }
    else if (numBlocks == 0) {
      numBlocks = 8 * blockSize;
    }
    numBlocks_ = static_cast<unsigned char>(numBlocks);
    assert(numBlocks_ == numBlocks);
}


FixedAllocator::~FixedAllocator() {
  for (Chunks::iterator itr = chunks_.begin(); itr != chunks_.end(); itr++) {
    delete &(*itr);
  }
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
  assert(!chunks_.empty());

  // linearly search for chunk to which p belongs starting from p and searching outward
  pDeallocChunk_ = findNearby(p);
  assert(pDeallocChunk_);
  DoDeallocate(p);
  /*
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
  */
}


size_t FixedAllocator::getBlockSize() const {
  return blockSize_;
}


Chunk* FixedAllocator::findNearby(void* p) {
  assert(!chunks_.empty());
  assert(pDeallocChunk_);
  const size_t chunkLength = numBlocks_ * blockSize_;
  Chunk* lo = pDeallocChunk_;
  Chunk* hi = pDeallocChunk_ + 1;
  Chunk* loBound = &chunks_.front();
  Chunk* hiBound = &chunks_.back() + 1;

  // special case: pDeallocChunk_ is the last in the vector
  if (hi == hiBound) {
    hi = 0;
  }

  for (;;) {
    if (lo) {
      // p lies in the address of chunk pointed to by lo
      if (p >= lo->pData_ && p < lo->pData_ + chunkLength) {
        return lo;
      }
      if (lo == loBound) {
        lo = 0;
      }
      else {
        --lo;
      }
    }
    if (hi) {
      if (p >= hi->pData_ && p < hi->pData_ + chunkLength) {
        return hi;
      }
      if (++hi == hiBound) {
        hi = 0;
      }
    }
  }
  // should never reach here
  return 0;
}


void FixedAllocator::DoDeallocate(void *p) {
  assert(p >= pDeallocChunk_->pData_);
  assert(p < (pDeallocChunk_->pData_ + numBlocks_ * blockSize_));

}
