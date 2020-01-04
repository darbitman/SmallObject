#pragma once

#include <cstdint>
#include <cstdlib>

class Chunk
{
  public:
    Chunk() noexcept;

    Chunk(Chunk&& other) noexcept = default;

    Chunk& operator=(Chunk&& other) noexcept = default;

    ~Chunk() noexcept = default;

    /// @brief Initialize Chunk memory address space
    /// @param blockSize The size of each object (in bytes)
    /// @param numBlocks The number of objects in this Chunk
    void Init(size_t blockSize, uint8_t numBlocks) noexcept;

    /// @brief Allocate memory and return a pointer to it to the client
    /// @param blockSize Size of the block to allocate memory for
    /// @return void* Returns a pointer to a block of size blockSize (in bytes). Returns a nullptr
    /// if no free memory exists or if the memory has not been initialized with the Init call.
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
    bool IsInChunk(const void* pBlock) const noexcept;

    /// Deleted to prevent misuse
    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;

  private:
    /// @brief Initialize the blocks in this Chunk in the form of a "linked list". Does not zero out
    /// memory.
    /// @param blockSize Size of each block (in bytes)
    /// @param numBlocks Total number of blocks in the memory space of this Chunk
    void Reset(size_t blockSize, uint8_t numBlocks);

    /// Points to the beginning of the memory space in this Chunk
    uint8_t* pData_;

    /// Points to the block past the last one in memory
    const uint8_t* pDataEnd_;

    uint8_t nextAvailableBlock_;

  public:
    uint8_t blocksAvailable_;
};
