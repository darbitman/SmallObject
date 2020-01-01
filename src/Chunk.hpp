#pragma once

#include <cstdint>
#include <cstdlib>
#include <stack>

class Chunk
{
  public:
    Chunk() noexcept = default;

    /// @brief initialize memory
    void Init(size_t blockSize, uint8_t numBlocks);

    // allocate memory and return a pointer to it to the client
    void* Allocate(size_t blockSize);

    // deallocate memory and add back to pool
    void Deallocate(void* p, size_t blockSize);

    // return memory back to OS
    void Release();

    uint8_t getNumBlocks() const;

  private:
    /// @brief Initialize blocks
    void Reset(size_t blockSize, uint8_t blocks);

    size_t blockSize_;

    uint8_t* pData_;

    uint8_t blocks_;

    uint8_t firstAvailableBlock_;

    uint8_t blocksAvailable_;
};
