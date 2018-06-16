//****************************************************************************//
// Chunk.h                                                                    //
// Author: Dmitriy A                                                          //
// Description:                                                               //
//    Low level memory manager for objects of a given size of bytes           //
//    Each chunk has a limited number of blocks                               //
//****************************************************************************//


#pragma once
#include <cstdlib>

class Chunk {
public:
  Chunk() {};

  // initialize memory
  void Init(size_t blockSize, unsigned char numBlocks);

  // allocate memory and return a pointer to it to the client
  void* Allocate(size_t blockSize);

  // deallocate memory and add back to pool
  void Deallocate(void* p, size_t blockSize);

  // initialize blocks
  void Reset(size_t blockSize, unsigned char blocks);

  // return memory back to OS
  void Release();

  unsigned char getNumBlocks() const;

  unsigned char* pData_;

  unsigned char
    firstAvailableBlock_,
    blocksAvailable_;

private:
  size_t blockSize_;

  unsigned char blocks_;
};
