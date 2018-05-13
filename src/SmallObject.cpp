#include "SmallObject.h"


void* SmallObject::operator new(size_t size) {
  return 0;
}


void SmallObject::operator delete(void* p, size_t size) {

}