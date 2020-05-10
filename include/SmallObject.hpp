#pragma once

#include <cstdlib>

namespace alloc {

class SmallObject {
 public:
  SmallObject() noexcept = default;

  virtual ~SmallObject() noexcept = default;

  /// @brief override call to OS for allocating memory for any class that derives from SmallObject
  static void* operator new(size_t size);

  /// @brief override call to OS for deallocating memory for any class that derives from SmallObject
  static void operator delete(void* p, size_t size);

 private:
  static constexpr size_t kMaxSmallObjectSize = 16;
};

}  // namespace alloc
