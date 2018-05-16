#include "SmallObject.h"
#include "SingletonHolder.h"
#include "SmallObjAllocator.h"
#include <chrono>
#include <iostream>
using namespace std::chrono;

class MySmallObject : public SmallObject {
public:
  int x;
};

int main() {
  MySmallObject* x = new MySmallObject();
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  x = new MySmallObject();
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  auto duration = duration_cast<nanoseconds>(t2 - t1).count();
  std::cout << duration << std::endl;

  t1 = high_resolution_clock::now();
  x = new MySmallObject();
  t2 = high_resolution_clock::now();
  duration = duration_cast<nanoseconds>(t2 - t1).count();
  std::cout << duration << std::endl;

  t1 = high_resolution_clock::now();
  x = new MySmallObject();
  t2 = high_resolution_clock::now();
  duration = duration_cast<nanoseconds>(t2 - t1).count();
  std::cout << duration << std::endl;

  t1 = high_resolution_clock::now();
  x = new MySmallObject();
  t2 = high_resolution_clock::now();
  duration = duration_cast<nanoseconds>(t2 - t1).count();
  std::cout << duration << std::endl;

  t1 = high_resolution_clock::now();
  x = new MySmallObject();
  t2 = high_resolution_clock::now();
  duration = duration_cast<nanoseconds>(t2 - t1).count();
  std::cout << duration << std::endl;


  x->x = 0xDEADBEEF;
  SmallObjAllocator& allocator = SingletonHolder<SmallObjAllocator, MAX_SMALL_OBJECT_SIZE>::getInstance();
  return 0;
}