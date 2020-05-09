#include "SmallObject.hpp"

#include "SingletonHolder.hpp"
#include "SmallObjAllocator.hpp"

namespace alloc {

void* SmallObject::operator new(size_t size) {
  return SingletonHolder<SmallObjAllocator>::GetInstance(kMaxSmallObjectSize).Allocate(size);
}

void SmallObject::operator delete(void* p, size_t size) {
  SingletonHolder<SmallObjAllocator>::GetInstance(kMaxSmallObjectSize).Deallocate(p, size);
}

}  // namespace alloc
