
#include "SmallObject.hpp"
#include "SingletonHolder.hpp"
#include "SmallObjAllocator.hpp"

void* SmallObject::operator new(size_t size)
{
    return SingletonHolder<SmallObjAllocator, MAX_SMALL_OBJECT_SIZE>::getInstance().Allocate(size);
}

void SmallObject::operator delete(void* p, size_t size)
{
    SingletonHolder<SmallObjAllocator, MAX_SMALL_OBJECT_SIZE>::getInstance().Deallocate(p, size);
}