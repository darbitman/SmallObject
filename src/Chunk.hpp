#pragma once

#include <cstdint>
#include <cstdlib>
#include <stack>

class Chunk
{
  public:
    Chunk() noexcept;

    /// @brief Initialize Chunk memory
    void Init(size_t blockSize, uint8_t numBlocks) noexcept;

    /// @brief Allocate memory and return a pointer to it to the client
    /// @param blockSize Size of the block to allocate memory for
    /// @return void* Returns a pointer to a block of size blockSize (in bytes). Returns a nullptr
    /// if no free memory exists.
    void* Allocate(size_t blockSize) noexcept;

    /// @brief Deallocate memory and add back to pool
    /// @param pBlock Pointer to the block to return to memory pool
    /// @param blockSize Size of each block (in bytes)
    /// @param numBlocks Total number of blocks in the memory space of this Chunk
    void Deallocate(void* pBlock, size_t blockSize, size_t numBlocks) noexcept;

    /// @brief Return memory back to OS
    void Release() noexcept;

    /// @brief Checks if the block pointed to by pBlock falls somewhere in the memory space of this
    /// Chunk
    /// @param pBlock The pointer to a block that needs to be checked
    /// @param blockSize Size of each block (in bytes)
    /// @param numBlocks Total number of blocks in the memory space of this Chunk
    bool IsInChunk(const void* pBlock, size_t blockSize, size_t numBlocks) const noexcept;

  private:
    /// Points to the beginning of the memory space in this Chunk
    uint8_t* pData_;

    /// @brief Initialize the blocks in this Chunk in the form of a "linked list". Does not zero out
    /// memory.
    /// @param blockSize Size of each block (in bytes)
    /// @param numBlocks Total number of blocks in the memory space of this Chunk
    void Reset(size_t blockSize, uint8_t numBlocks);

    uint8_t* pDataEnd_;
    uint8_t nextAvailableBlock_;

  public:
    uint8_t blocksAvailable_;
};
