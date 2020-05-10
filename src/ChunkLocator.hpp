#pragma once

#include <vector>

#include "CommonTypes.hpp"

namespace alloc::util {

/// @brief Finds the Chunk that performed a memory allocation for an object pointed to by p_object. This Chunk is one
/// of potentially many in the chunk_container
/// @param p_object Points at an object that was allocated by a Chunk in the chunk_container
/// @param starting_chunk Should be a reference to a Chunk in the container. This provides a good starting point if
/// the client keeps track of LRU Chunks. If this information is not kept, then can pass a reference to any Chunk in
/// the chunk_container
/// @param chunk_container Should not be an empty container. This should contain a Chunk that allocated the memory
/// pointed to by p_object
/// @return Chunk* A valid Chunk object that allocated the memory pointed to by p_object.
Chunk* FindChunkOwner(void* p_object, Chunk& starting_chunk, const Chunks& chunk_container) noexcept;

}  // namespace alloc::util
