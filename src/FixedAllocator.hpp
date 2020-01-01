//****************************************************************************//
// FixedAllocator.h                                                           //
// Author: Dmitriy A                                                          //
// Description:                                                               //
//    Allocates objects of a given size                                       //
//    Caches LRU chunk for alloc and dealloc                                  //
//****************************************************************************//


#pragma once
#include <cstdlib>
#include <vector>
#include "Chunk.h"

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 48
#endif

class FixedAllocator {
public:
  // create object to handle objects of blockSize bytes
  explicit FixedAllocator(size_t blockSize = 0);

  // release chunk back to OS
  ~FixedAllocator();

  // allocate memory and return pointer
  void* Allocate();

  // return memory back to chunk handler
  void Deallocate(void* p);

  size_t getBlockSize() const;

private:
  size_t blockSize_;
  unsigned char numBlocks_;

  // holds all chunks that handle blockSize requests
  typedef std::vector<Chunk> Chunks;
  Chunks chunks_;

  // LRU chunk that handled an alloc
  Chunk* pAllocChunk_;

  // LRU chunk that handled a dealloc
  Chunk* pDeallocChunk_;

  // find chunk that allocated pointer p
  Chunk* findNearby(void* p);

  // perform the deallocation of pointer p
  void DoDeallocate(void* p);
};
