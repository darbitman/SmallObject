#pragma once
#include <cstdlib>

class SmallObject {
public:
  static void* operator new(size_t size);
  static void operator delete(void* p, size_t size);
  virtual ~SmallObject() {}
};