#pragma once

#include <cstdlib>

template <typename SingletonType, size_t maxObjectSize>
class SingletonHolder
{
  public:
    static SingletonType& getInstance()
    {
        static SingletonType instance(maxObjectSize);
        return instance;
    }

    SingletonHolder(const SingletonHolder<SingletonType, maxObjectSize>&) = delete;
    SingletonHolder(SingletonHolder<SingletonType, maxObjectSize>&&) = delete;
    SingletonHolder& operator=(const SingletonHolder<SingletonType, maxObjectSize>&) = delete;
    SingletonHolder& operator=(SingletonHolder<SingletonType, maxObjectSize>&&) = delete;

  private:
    SingletonHolder() noexcept = default;
    ~SingletonHolder() noexcept = default;
};