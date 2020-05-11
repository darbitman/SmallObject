#include <gtest/gtest.h>

#include "ChunkManager.hpp"

namespace alloc {
namespace {

TEST(ChunkManagerTest, VerifyBlockSize) {
  ChunkManager cm(8);

  EXPECT_EQ(cm.GetBlockSize(), 8);
}

TEST(ChunkManagerTest, VerifyNumBlocks) {
  ChunkManager cm(8);

  auto& chunk = cm.CreateNewChunk();

  EXPECT_EQ(chunk.GetNumBlocksAvailable(), cm.GetNumBlocks());
}

TEST(ChunkManagerTest, LocateChunkOwnerAmongSingleChunk) {
  ChunkManager cm(8);

  auto& chunk = cm.CreateNewChunk();

  auto p_block = chunk.Allocate(8);

  auto p_chunk_owner = cm.FindChunkOwner(p_block, chunk);

  EXPECT_EQ(p_chunk_owner, &chunk);
}

TEST(ChunkManagerTest, FindChunkOwnerAmongManyChunks) {
  ChunkManager cm(8);

  // create 4 Chunks
  cm.CreateNewChunk();
  cm.CreateNewChunk();
  cm.CreateNewChunk();
  auto& chunk = cm.CreateNewChunk();

  auto p_block = chunk.Allocate(8);

  auto p_chunk_owner = cm.FindChunkOwner(p_block, chunk);

  EXPECT_EQ(p_chunk_owner, &chunk);
}

TEST(ChunkManagerTest, FindFreeChunk) {
  ChunkManager cm(8);

  // create first Chunk, allocate all of its memory, and save pointer to be able to retrieve the Chunk it belongs to
  // later
  auto p_block_in_first_chunk = [&]() {
    // create 2 Chunks
    auto& chunk_one = cm.CreateNewChunk();

    auto p_first_block = chunk_one.Allocate(8);

    // allocate all of the first Chunk's memory
    while (chunk_one.Allocate(8) != nullptr)
      ;

    return p_first_block;
  }();

  // create a second Chunk
  cm.CreateNewChunk();

  auto& free_chunk = cm.FindFreeChunk();

  auto p_chunk_owner = cm.FindChunkOwner(p_block_in_first_chunk, free_chunk);

  // the Chunk owner should not be the free Chunk since all of the owner's memory was allocated
  EXPECT_NE(&free_chunk, p_chunk_owner);
}

}  // namespace
}  // namespace alloc
