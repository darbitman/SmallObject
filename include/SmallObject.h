//****************************************************************************//
// SmallObject.h                                                              //
// Author: Dmitriy A                                                          //
// Description:                                                               //
//    Handles allocations by override new and delete operators                //
//****************************************************************************//


#pragma once
#include <cstdlib>

#define MAX_SMALL_OBJECT_SIZE 16

class SmallObject {
public:
  // override call to OS
  static void* operator new(size_t size);

  // override call to OS
  static void operator delete(void* p, size_t size);

  virtual ~SmallObject() {}
};