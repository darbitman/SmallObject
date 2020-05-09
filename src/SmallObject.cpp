#include "SmallObject.hpp"

#include "SingletonHolder.hpp"
#include "SmallObjAllocator.hpp"

namespace alloc {

void* SmallObject::operator new(size_t size) {
  return SingletonHolder<SmallObjAllocator, kMaxSmallObjectSize>::getInstance().Allocate(size);
}

void SmallObject::operator delete(void* p, size_t size) {
  SingletonHolder<SmallObjAllocator, kMaxSmallObjectSize>::getInstance().Deallocate(p, size);
}

}