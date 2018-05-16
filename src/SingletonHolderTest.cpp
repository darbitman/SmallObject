#include "SingletonHolder.h"
#include "SmallObjAllocator.h"
#include <vector>
#include <iostream>


struct SmallObjTestStruct {
  unsigned int a;
  unsigned int b;
  unsigned int c;
};

struct SmallObjTestStruct2 {
  unsigned int a;
  unsigned int b;
  unsigned int c;
  unsigned int d;
};

int main() {
  const size_t maxObjectSize = 16;
  unsigned int numBlocks = 3;
  unsigned int numChunks = 4;
  unsigned int numTotalAllocations = numBlocks * numChunks;
  void** chunkPtrArray1 = new void*[numTotalAllocations];
  // allocate 4 chunks of 3 blocks each
  HANDLE threads[12];
  DWORD ThreadID;
  SmallObjAllocator a = SingletonHolder<SmallObjAllocator, maxObjectSize>::getInstance();
  for (unsigned int i = 0; i < numTotalAllocations; i++) {
    threads[i] = CreateThread(
      NULL,
      0,
      (LPTHREAD_START_ROUTINE) (SingletonHolder<SmallObjAllocator, maxObjectSize>::getInstance().Allocate(sizeof(SmallObjTestStruct)),
      NULL,
      0,
      &ThreadID);
  }

  WaitForMultipleObjects(12, threads, TRUE, INFINITE);

  //for (unsigned int i = 0; i < numTotalAllocations; i++) {
  //  chunkPtrArray1[i] = SingletonHolder<SmallObjAllocator, maxObjectSize>::getInstance().Allocate(sizeof(SmallObjTestStruct));
  //}


  //void** chunkPtrArray2 = new void*[numTotalAllocations];
  //// allocate 4 chunks of 3 blocks each
  //for (unsigned int i = 0; i < numTotalAllocations; i++) {
  //  chunkPtrArray2[i] = SingletonHolder<SmallObjAllocator, maxObjectSize>::getInstance().Allocate(sizeof(SmallObjTestStruct));
  //}

  return 0;
}