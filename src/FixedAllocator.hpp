#pragma once

#include <cstdlib>
#include <vector>

#include "Chunk.hpp"

class FixedAllocator
{
  public:
    /// @brief Create a FixedAlloctor object to handle allocating memory to objects of blockSize
    /// bytes
    /// @param blockSize Size of objects (in bytes)
    explicit FixedAllocator(size_t blockSize) noexcept;

    FixedAllocator& operator=(FixedAllocator&&) noexcept = default;

    /// @brief release managed Chunks back to OS
    ~FixedAllocator() noexcept = default;

    /// @brief Allocate memory and return pointer to the block of memory
    void* Allocate() noexcept;

    /// @brief Return memory back to Chunk handler
    void Deallocate(void* pObject) noexcept;

    /// @brief Return the size (in bytes) of the objects that this FixedAllocator allocates
    size_t getBlockSize() const noexcept;

    /// Deleted to prevent misuse
    FixedAllocator(const FixedAllocator&) noexcept = delete;
    FixedAllocator(FixedAllocator&&) noexcept = default;
    FixedAllocator& operator=(const FixedAllocator&) noexcept = delete;

  private:
    using Chunks = std::vector<Chunk>;

    /// The default size (in bytes) of a Chunk
    static constexpr size_t DEFAULT_CHUNK_SIZE = 4096;

    /// @brief Find Chunk that allocated pointer pObject
    /// @param pObject
    Chunk* FindChunkOwner(void* pObject) noexcept;

    /// @brief Perform the deallocation of pointer pObject
    void DoDeallocate(void* pObject) noexcept;

    // LRU chunk that handled an alloc
    Chunk* pAllocChunk_;

    // LRU chunk that handled a dealloc
    Chunk* pDeallocChunk_;

    size_t blockSize_;

    uint8_t numBlocks_;

    // holds all chunks that handle blockSize requests
    Chunks chunks_;
};
