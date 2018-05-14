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
  unsigned int maxChunkSize = 255;
  unsigned int numChunks = 4;
  unsigned int numTotalAllocations = maxChunkSize * numChunks;
  void** chunkPtrArr = new void*[numTotalAllocations];
  FixedAllocator fa1(sizeof(ChunkTestStruct));
  for (unsigned int i = 0; i < numTotalAllocations; i++) {
    chunkPtrArr[i] = fa1.Allocate();
  }
  for (int i = numTotalAllocations - 1; i >= 0; i--) {
    fa1.Deallocate(chunkPtrArr[i]);
  }
  return 0;
}