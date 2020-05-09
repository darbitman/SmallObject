#include <gtest/gtest.h>

#include <vector>

#include "Chunk.hpp"

class ChunkTest : public ::testing::Test
{
  protected:
    static constexpr size_t DEFAULT_BLOCK_SIZE = 8;
    static constexpr uint8_t DEFAULT_NUM_BLOCKS = 10;

    Chunk c;
};

TEST_F(ChunkTest, Initialize)
{
    c.Init(DEFAULT_BLOCK_SIZE, DEFAULT_NUM_BLOCKS);

    EXPECT_EQ(c.blocksAvailable_, DEFAULT_NUM_BLOCKS);

    c.Release();

    EXPECT_EQ(c.blocksAvailable_, 0);
}

TEST_F(ChunkTest, AllocateWithoutInitialize) { EXPECT_EQ(c.Allocate(DEFAULT_BLOCK_SIZE), nullptr); }

TEST_F(ChunkTest, Allocate_Deallocate)
{
    c.Init(DEFAULT_BLOCK_SIZE, DEFAULT_NUM_BLOCKS);

    auto pAllocatedBlock = c.Allocate(DEFAULT_BLOCK_SIZE);

    EXPECT_EQ(c.blocksAvailable_, DEFAULT_NUM_BLOCKS - 1);

    EXPECT_EQ(c.IsInChunk(pAllocatedBlock), true);

    c.Deallocate(pAllocatedBlock, DEFAULT_BLOCK_SIZE);

    EXPECT_EQ(c.blocksAvailable_, DEFAULT_NUM_BLOCKS);

    c.Release();
}

TEST_F(ChunkTest, AllocateAll_DeallocateAll)
{
    c.Init(DEFAULT_BLOCK_SIZE, DEFAULT_NUM_BLOCKS);

    std::vector<void*> allocatedBlocks;

    allocatedBlocks.reserve(DEFAULT_NUM_BLOCKS);

    for (uint8_t i = 0; i < DEFAULT_NUM_BLOCKS; ++i)
    {
        allocatedBlocks.push_back(c.Allocate(DEFAULT_BLOCK_SIZE));
    }

    EXPECT_EQ(c.blocksAvailable_, 0);

    EXPECT_EQ(c.Allocate(DEFAULT_BLOCK_SIZE), nullptr);

    for (auto& pBlock : allocatedBlocks)
    {
        c.Deallocate(pBlock, DEFAULT_BLOCK_SIZE);
    }

    EXPECT_EQ(c.blocksAvailable_, DEFAULT_NUM_BLOCKS);

    c.Release();
}
