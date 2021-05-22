#include <benchmark/benchmark.h>

#include "Chunk.hpp"

namespace alloc {
namespace {

void BM_ChunkAllocate(benchmark::State& state) {
  std::vector<void*> v;
  v.reserve(255);

  Chunk c{};
  c.Init(8, 255);

  for (auto _ : state) {
    state.PauseTiming();
    v.clear();
    state.ResumeTiming();

    for (uint8_t i = 0; i < uint8_t{255}; ++i) {
      v.push_back(c.Allocate(8));
    }

    for (auto ptr : v) {
      c.Deallocate(ptr, 8);
    }
  }
}

void BM_Malloc(benchmark::State& state) {
  std::vector<void*> v;
  v.reserve(255);

  for (auto _ : state) {
    state.PauseTiming();
    v.clear();
    state.ResumeTiming();

    for (uint8_t i = 0; i < uint8_t{123}; ++i) {
      v.push_back(::operator new(8));
    }

    for (auto ptr : v) {
      ::operator delete(ptr);
    }
  }
}

BENCHMARK(BM_ChunkAllocate);
BENCHMARK(BM_Malloc);

}  // namespace
}  // namespace alloc
