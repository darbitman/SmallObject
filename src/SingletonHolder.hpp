#pragma once

#include <cstdlib>
#include <utility>

template <typename SingletonType>
class SingletonHolder {
 public:
  template <typename... Types>
  static SingletonType& GetInstance(Types&&... constructor_args);

  SingletonHolder() noexcept                             = delete;
  ~SingletonHolder() noexcept                            = delete;
  SingletonHolder(const SingletonHolder<SingletonType>&) = delete;
  SingletonHolder(SingletonHolder<SingletonType>&&)      = delete;
  SingletonHolder& operator=(const SingletonHolder<SingletonType>&) = delete;
  SingletonHolder& operator=(SingletonHolder<SingletonType>&&) = delete;

 private:
};

template <typename SingletonType>
template <typename... Types>
SingletonType& SingletonHolder<SingletonType>::GetInstance(Types&&... constructor_args) {
  static SingletonType instance(std::forward<Types>(constructor_args)...);
  return instance;
}
