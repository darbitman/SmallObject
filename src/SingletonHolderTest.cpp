#include "SingletonHolder.h"
#include "SmallObjAllocator.h"

int main() {
  SingletonHolder<SmallObjAllocator>::getInstance();
  //size_t chunkSize = 1024;
  //size_t maxObjectSize = 16;
  //SmallObjAllocator soa1(chunkSize, maxObjectSize);

  //unsigned int numBlocks = 3;
  //unsigned int numChunks = 4;
  //unsigned int numTotalAllocations = numBlocks * numChunks;
  //void** chunkPtrArray1 = new void*[numTotalAllocations];
  //// allocate 4 chunks of 3 blocks each
  //for (unsigned int i = 0; i < numTotalAllocations; i++) {
  //  chunkPtrArray1[i] = soa1.Allocate(sizeof(SmallObjTestStruct));
  //}


  //void** chunkPtrArray2 = new void*[numTotalAllocations];
  //// allocate 4 chunks of 3 blocks each
  //for (unsigned int i = 0; i < numTotalAllocations; i++) {
  //  chunkPtrArray2[i] = soa1.Allocate(sizeof(SmallObjTestStruct2));
  //}
}