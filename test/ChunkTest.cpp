#include <gtest/gtest.h>

#include <vector>

#include "Chunk.hpp"

class ChunkTest : public ::testing::Test {
 protected:
  static constexpr size_t  kDefaultBlockSize = 8;
  static constexpr uint8_t kDefaultNumBlocks = 10;

  Chunk c;
};

TEST_F(ChunkTest, Initialize) {
  c.Init(kDefaultBlockSize, kDefaultNumBlocks);

  EXPECT_EQ(c.blocksAvailable_, kDefaultNumBlocks);

  c.Release();

  EXPECT_EQ(c.blocksAvailable_, 0);
}

TEST_F(ChunkTest, AllocateWithoutInitialize) { EXPECT_EQ(c.Allocate(kDefaultBlockSize), nullptr); }

TEST_F(ChunkTest, Allocate_Deallocate) {
  c.Init(kDefaultBlockSize, kDefaultNumBlocks);

  auto pAllocatedBlock = c.Allocate(kDefaultBlockSize);

  EXPECT_EQ(c.blocksAvailable_, kDefaultNumBlocks - 1);

  EXPECT_EQ(c.IsInChunk(pAllocatedBlock), true);

  c.Deallocate(pAllocatedBlock, kDefaultBlockSize);

  EXPECT_EQ(c.blocksAvailable_, kDefaultNumBlocks);

  c.Release();
}

TEST_F(ChunkTest, AllocateAll_DeallocateAll) {
  c.Init(kDefaultBlockSize, kDefaultNumBlocks);

  std::vector<void*> allocatedBlocks;

  allocatedBlocks.reserve(kDefaultNumBlocks);

  for (uint8_t i = 0; i < kDefaultNumBlocks; ++i) {
    allocatedBlocks.push_back(c.Allocate(kDefaultBlockSize));
  }

  EXPECT_EQ(c.blocksAvailable_, 0);

  EXPECT_EQ(c.Allocate(kDefaultBlockSize), nullptr);

  for (auto& pBlock : allocatedBlocks) {
    c.Deallocate(pBlock, kDefaultBlockSize);
  }

  EXPECT_EQ(c.blocksAvailable_, kDefaultNumBlocks);

  c.Release();
}
