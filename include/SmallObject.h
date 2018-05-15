#pragma once
#include <cstdlib>

#define MAX_SMALL_OBJECT_SIZE 16

class SmallObject {
public:
  static void* operator new(size_t size);
  static void operator delete(void* p, size_t size);
  virtual ~SmallObject() {}
};