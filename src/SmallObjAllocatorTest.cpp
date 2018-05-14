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
  SmallObjAllocator a(chunkSize, maxObjectSize);
  void* x = a.Allocate(sizeof(SmallObjTestStruct));
  void* y = a.Allocate(sizeof(SmallObjTestStruct2));
  a.Deallocate(x, sizeof(SmallObjTestStruct));
  a.Deallocate(y, sizeof(SmallObjTestStruct2));
}