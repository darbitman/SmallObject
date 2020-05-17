#include "Chunk.hpp"

namespace alloc {

Chunk::Chunk() noexcept : p_data_(nullptr), p_data_end_(nullptr), next_available_block_(0), blocks_available_(0) {}

void Chunk::Init(size_t block_size, uint8_t num_blocks) noexcept {
  // allocate new memory
  p_data_ = new uint8_t[block_size * num_blocks];

  // update data end pointer. Points at past-the-end block
  p_data_end_ = p_data_ + (block_size * num_blocks);

  // initialize blocks
  Reset(block_size, num_blocks);
}

void* Chunk::Allocate(size_t block_size) noexcept {
  if (blocks_available_ == 0) {
    return nullptr;
  }

  uint8_t* resultPtr = p_data_ + (next_available_block_ * block_size);

  // Update nextAvailableBlock_ to point to the next block
  next_available_block_ = *resultPtr;
  --blocks_available_;
  return resultPtr;
}

void Chunk::Deallocate(void* p_block, size_t block_size) noexcept {
  uint8_t* pBlockToRelease = static_cast<uint8_t*>(p_block);

  // update current block's "list pointer" to point to the block that was previously the first available block
  *pBlockToRelease = next_available_block_;

  // set the first available block to "point" at the block that is being deallocated
  next_available_block_ = static_cast<uint8_t>((pBlockToRelease - p_data_) / block_size);

  ++blocks_available_;
}

void Chunk::Reset(size_t block_size, uint8_t num_blocks) {
  next_available_block_ = 0;

  blocks_available_ = num_blocks;

  uint8_t* p_data = p_data_;
  for (uint8_t i = 0; i < num_blocks; p_data += block_size) {
    // assign first byte in each block to the index of the next block (ie contiguous linked list)
    // last block's 'pointer' points at past-the-end block
    *p_data = ++i;
  }
}

void Chunk::Release() noexcept {
  if (p_data_ != nullptr) {
    delete[] p_data_;
    p_data_               = nullptr;
    p_data_end_           = nullptr;
    next_available_block_ = 0;
    blocks_available_     = 0;
  }
}

bool Chunk::IsInChunk(const void* p_block) const noexcept {
  auto p_block_to_check = static_cast<const uint8_t*>(p_block);

  // check if p_block falls within the memory space of this Chunk
  return (p_block_to_check >= p_data_) && (p_block_to_check < p_data_end_);
}

uint8_t Chunk::GetNumBlocksAvailable() const noexcept { return blocks_available_; }

}  // namespace alloc
