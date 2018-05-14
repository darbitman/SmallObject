#include "SmallObjAllocator.h"
#include <assert.h>

struct SmallObjTestStruct {
  unsigned int a;
  unsigned int b;
  unsigned int c;
};

struct SmallObjTestStruct2{
  unsigned int a;
  unsigned int b;
  unsigned int c;
  unsigned int d;
};

int main() {
  size_t chunkSize = 1024;
  size_t maxObjectSize = 16;
  SmallObjAllocator soa1(chunkSize, maxObjectSize);
  
  unsigned int numBlocks = 3;
  unsigned int numChunks = 4;
  unsigned int numTotalAllocations = numBlocks * numChunks;
  void** chunkPtrArray1 = new void*[numTotalAllocations];
  // allocate 4 chunks of 3 blocks each
  for (unsigned int i = 0; i < numTotalAllocations; i++) {
    chunkPtrArray1[i] = soa1.Allocate(sizeof(SmallObjTestStruct));
  }


  void** chunkPtrArray2 = new void*[numTotalAllocations];
  // allocate 4 chunks of 3 blocks each
  for (unsigned int i = 0; i < numTotalAllocations; i++) {
    chunkPtrArray2[i] = soa1.Allocate(sizeof(SmallObjTestStruct2));
  }
}