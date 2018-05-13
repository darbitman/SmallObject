#pragma once
#include <cstdlib>

template<typename SingletonType>
class SingletonHolder {
public:
  static SingletonType& getInstance() {
    static SingletonType instance;
    return instance;
  }

  SingletonHolder(const SingletonHolder<SingletonType>&) = delete;
  void operator=(const SingletonHolder<SingletonType>&) = delete;
private:
  SingletonHolder() {}
  ~SingletonHolder() {}
};