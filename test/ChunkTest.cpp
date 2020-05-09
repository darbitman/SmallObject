#include <gtest/gtest.h>

#include <vector>

#include "Chunk.hpp"

namespace alloc {
namespace {

class ChunkTest : public ::testing::Test {
 protected:
  static constexpr size_t  kDefaultBlockSize = 8;
  static constexpr uint8_t kDefaultNumBlocks = 10;

  Chunk c;
};

TEST(ChunkTest, Uninitialized) {
  Chunk c;

  EXPECT_EQ(c.GetNumBlocksAvailable(), 0);

  EXPECT_EQ(c.Allocate(8), nullptr);
}

TEST(ChunkTest, Initialize) {
  Chunk c;

  c.Init(8, 20);

  EXPECT_EQ(c.GetNumBlocksAvailable(), 20);
}

TEST(ChunkTest, AllocateDeallocate) {
  Chunk c;

  c.Init(8, 10);

  auto pAllocatedBlock = c.Allocate(8);

  EXPECT_EQ(c.GetNumBlocksAvailable(), 9);

  EXPECT_EQ(c.IsInChunk(pAllocatedBlock), true);

  c.Deallocate(pAllocatedBlock, 8);

  EXPECT_EQ(c.GetNumBlocksAvailable(), 10);
}

TEST(ChunkTest, AllocateAll) {
  Chunk c;

  c.Init(8, 10);

  std::vector<void*> allocated_blocks;

  allocated_blocks.reserve(10);

  for (uint8_t i = 0; i < 10; ++i) {
    allocated_blocks.push_back(c.Allocate(8));
  }

  EXPECT_EQ(c.GetNumBlocksAvailable(), 0);

  EXPECT_EQ(c.Allocate(8), nullptr);

  for (auto& p_block : allocated_blocks) {
    c.Deallocate(p_block, 8);
  }

  EXPECT_EQ(c.GetNumBlocksAvailable(), 10);
}

TEST(ChunkTest, ReleasingMemory) {
  Chunk c;

  c.Release();

  EXPECT_EQ(c.Allocate(8), nullptr);

  c.Init(8, 10);
  c.Release();

  EXPECT_EQ(c.Allocate(8), nullptr);
}

}  // namespace
}  // namespace alloc
