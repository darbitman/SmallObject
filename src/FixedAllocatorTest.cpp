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
  FixedAllocator fa1(sizeof(ChunkTestStruct));
  ChunkTestStruct* pOne = reinterpret_cast<ChunkTestStruct*>(fa1.Allocate());
  pOne->a = 10;
  pOne->b = 20;
  pOne->c = 30;
  fa1.Deallocate(pOne);
  return 0;
}