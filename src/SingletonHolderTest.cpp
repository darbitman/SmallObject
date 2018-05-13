#include "SingletonHolder.h"
#include "SmallObjAllocator.h"

int main() {
  SingletonHolder<SmallObjAllocator>::getInstance();
}