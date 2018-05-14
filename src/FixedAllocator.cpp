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
    assert(itr->getNumBlocks() == numBlocks_);
    itr->Release();
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

  // special case: pDeallocChunk_ is the last Chunk in the vector
  if (hi == hiBound) {
    hi = 0;
  }

  for (;;) {
    if (lo) {
      // p lies in the address space of a Chunk pointed to by lo
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
  pDeallocChunk_->Deallocate(p, blockSize_);
  if (pDeallocChunk_->blocksAvailable_ == numBlocks_) {
    // pDeallocChunk has all blocks freed
    // check if another chunk is empty in order to release a Chunk
    Chunk& lastChunk = chunks_.back();
    if (&lastChunk == pDeallocChunk_) {
      if (chunks_.size() > 1 &&
        pDeallocChunk_[-1].blocksAvailable_ == numBlocks_) {
        // have more than 1 chunk and last 2 chunks are empty
        // can delete the last one
        chunks_.back().Release();
        chunks_.pop_back();
        pAllocChunk_ = pDeallocChunk_ = &chunks_.front();
      }
      return;
    }
    // pDeallocChunk and lastChunk are not the same Chunk
    // checking if lastChunk is empty
    // if empty, release it and change pointer pAllocChunk
    //    to point to pDeallocChunk since it's empty
    // move empty chunk to the back
    if (lastChunk.blocksAvailable_ == numBlocks_) {
      chunks_.back().Release();
      chunks_.pop_back();
      pAllocChunk_ = pDeallocChunk_;
      std::swap(*pDeallocChunk_, chunks_.back());
    }
    // if last chunk isn't empty, move empty chunk to the back
    // set pAllocChunk to point to it since that's where free blocks
    //    will be allocated from
    else {
      std::swap(*pDeallocChunk_, lastChunk);
      pAllocChunk_ = &chunks_.back();
    }
  }
}
