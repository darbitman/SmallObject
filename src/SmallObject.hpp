#pragma once

#include <cstdlib>

#include "SmallObjAllocator.hpp"

class SmallObject
{
  public:
    SmallObject() noexcept = default;

    virtual ~SmallObject() noexcept = default;

    // override call to OS
    static void* operator new(size_t size);

    // override call to OS
    static void operator delete(void* p, size_t size);

  private:
    static constexpr size_t MAX_SMALL_OBJECT_SIZE = 16;
};