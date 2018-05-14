#include "Chunk.h"
#include <assert.h>
#include <iostream>

using std::cout;

struct ChunkTestStruct {
  unsigned int a;
  unsigned int b;
  unsigned int c;
};

int main() {
  Chunk a;
  a.Init(sizeof(ChunkTestStruct), 3);
  void* x1 = a.Allocate(sizeof(ChunkTestStruct));
  void* x2 = a.Allocate(sizeof(ChunkTestStruct));
  void* x3 = a.Allocate(sizeof(ChunkTestStruct));
  //Chunk a;
  //a.Init(sizeof(ChunkTestStruct), 3);
  //ChunkTestStruct* pTestStructA = reinterpret_cast<ChunkTestStruct*>(a.Allocate(sizeof(ChunkTestStruct)));
  //pTestStructA->a = 10;
  //pTestStructA->b = 20;
  //pTestStructA->c = 30;
  //assert(pTestStructA->a == 10);
  //assert(pTestStructA->b == 20);
  //assert(pTestStructA->c == 30);
  //a.Deallocate(pTestStructA, sizeof(ChunkTestStruct));
  //ChunkTestStruct* pTestStructB = reinterpret_cast<ChunkTestStruct*>(a.Allocate(sizeof(ChunkTestStruct)));
  //pTestStructB->a = 0x1000;
  //pTestStructB->b = 0x2000;
  //pTestStructB->c = 0x3000;
  //assert(pTestStructB->a == 0x1000);
  //assert(pTestStructB->b == 0x2000);
  //assert(pTestStructB->c == 0x3000);
  //a.Deallocate(pTestStructB, sizeof(ChunkTestStruct));

  //pTestStructA = reinterpret_cast<ChunkTestStruct*>(a.Allocate(sizeof(ChunkTestStruct)));
  //pTestStructB = reinterpret_cast<ChunkTestStruct*>(a.Allocate(sizeof(ChunkTestStruct)));
  //pTestStructA->a = 10;
  //pTestStructA->b = 20;
  //pTestStructA->c = 30;
  //assert(pTestStructA->a == 10);
  //assert(pTestStructA->b == 20);
  //assert(pTestStructA->c == 30);
  //pTestStructB->a = 0x1000;
  //pTestStructB->b = 0x2000;
  //pTestStructB->c = 0x3000;
  //assert(pTestStructB->a == 0x1000);
  //assert(pTestStructB->b == 0x2000);
  //assert(pTestStructB->c == 0x3000);
  //a.Deallocate(pTestStructA, sizeof(ChunkTestStruct));
  //a.Deallocate(pTestStructB, sizeof(ChunkTestStruct));
}