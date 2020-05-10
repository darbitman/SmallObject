#include <gtest/gtest.h>

#include "AllocatorUtility.hpp"

namespace alloc {
namespace {

TEST(FindChunkOwner, SingleChunk) {
  Chunks chunks;
  chunks.emplace_back();        // create Chunk
  chunks.front().Init(8, 100);  // initialize the Chunk

  auto* p_object = chunks.front().Allocate(8);

  auto* p_chunk = util::FindChunkOwner(p_object, chunks.front(), chunks);

  EXPECT_TRUE(p_chunk->IsInChunk(p_object));
}

TEST(FindChunkOwner, ManyChunksAllocWithFirst) {
  Chunks chunks;

  // create 5 Chunk objects and initialize them
  chunks.emplace_back();
  chunks.back().Init(8, 100);
  chunks.emplace_back();
  chunks.back().Init(8, 100);
  chunks.emplace_back();
  chunks.back().Init(8, 100);
  chunks.emplace_back();
  chunks.back().Init(8, 100);
  chunks.emplace_back();
  chunks.back().Init(8, 100);

  // allocate using first Chunk
  auto const p_object = chunks.front().Allocate(8);

  // set starting point of the search to the last Chunk
  auto p_chunk = util::FindChunkOwner(p_object, chunks.back(), chunks);
  EXPECT_TRUE(p_chunk->IsInChunk(p_object));

  // set starting point of the search to a Chunk in the middle
  p_chunk = util::FindChunkOwner(p_object, chunks.at(2), chunks);
  EXPECT_TRUE(p_chunk->IsInChunk(p_object));
}

TEST(FindChunkOwner, ManyChunksAllocWithMiddle) {
  Chunks chunks;

  // create 5 Chunk objects and initialize them
  chunks.emplace_back();
  chunks.back().Init(8, 100);
  chunks.emplace_back();
  chunks.back().Init(8, 100);
  chunks.emplace_back();
  chunks.back().Init(8, 100);
  chunks.emplace_back();
  chunks.back().Init(8, 100);
  chunks.emplace_back();
  chunks.back().Init(8, 100);

  // allocate using middle Chunk
  auto const p_object = chunks.at(2).Allocate(8);

  // set starting point of the search to the last Chunk
  auto p_chunk = util::FindChunkOwner(p_object, chunks.back(), chunks);
  EXPECT_TRUE(p_chunk->IsInChunk(p_object));

  // set starting point of the search to the first Chunk
  p_chunk = util::FindChunkOwner(p_object, chunks.front(), chunks);
  EXPECT_TRUE(p_chunk->IsInChunk(p_object));
}

TEST(FindChunkOwner, ManyChunksAllocWithLast) {
  Chunks chunks;

  // create 5 Chunk objects and initialize them
  chunks.emplace_back();
  chunks.back().Init(8, 100);
  chunks.emplace_back();
  chunks.back().Init(8, 100);
  chunks.emplace_back();
  chunks.back().Init(8, 100);
  chunks.emplace_back();
  chunks.back().Init(8, 100);
  chunks.emplace_back();
  chunks.back().Init(8, 100);

  // allocate using last Chunk
  auto const p_object = chunks.back().Allocate(8);

  // set starting point of the search to the first Chunk
  auto p_chunk = util::FindChunkOwner(p_object, chunks.front(), chunks);
  EXPECT_TRUE(p_chunk->IsInChunk(p_object));

  // set starting point of the search to a Chunk in the middle
  p_chunk = util::FindChunkOwner(p_object, chunks.at(2), chunks);
  EXPECT_TRUE(p_chunk->IsInChunk(p_object));
}

}  // namespace
}  // namespace alloc
