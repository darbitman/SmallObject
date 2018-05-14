#include "FixedAllocator.h"
#include <assert.h>
#include <iostream>

using std::cout;

struct ChunkTestStruct {
  unsigned int a;
  unsigned int b;
  unsigned int c;
};

int main() {
  unsigned int numBlocks = 3;
  unsigned int numChunks = 4;
  unsigned int numTotalAllocations = numBlocks * numChunks;
  void** chunkPtrArr = new void*[numTotalAllocations];
  FixedAllocator fa1(sizeof(ChunkTestStruct));
  // allocates 4 chunks of 3 blocks each
  for (unsigned int i = 0; i < numTotalAllocations; i++) {
    chunkPtrArr[i] = fa1.Allocate();
  }

  // deallocate 2nd chunk and move to back
  fa1.Deallocate(chunkPtrArr[5]);
  fa1.Deallocate(chunkPtrArr[4]);
  fa1.Deallocate(chunkPtrArr[3]);
  
  // deallocate last chunk, release last chunk's (2nd) memory, and move this one to the back
  fa1.Deallocate(chunkPtrArr[11]);
  fa1.Deallocate(chunkPtrArr[10]);
  fa1.Deallocate(chunkPtrArr[9]);
  return 0;
}