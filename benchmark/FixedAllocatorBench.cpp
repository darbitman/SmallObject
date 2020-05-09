#include <benchmark/benchmark.h>

#include "FixedAllocator.hpp"

static void BM_AllocateUsingFixedAllocator(benchmark::State& state)
{
    FixedAllocator allocator(size_t(state.range(0)));

    for (auto _ : state)
    {
        auto* p_memory = allocator.Allocate();
        benchmark::DoNotOptimize(p_memory);
    }
}

BENCHMARK(BM_AllocateUsingFixedAllocator)->RangeMultiplier(2)->Range(4, 64);

static void BM_AllocateUsingOpNew(benchmark::State& state)
{
    size_t object_size = size_t(state.range(0));
    for (auto _ : state)
    {
        auto* p_memory = ::operator new(object_size);
        benchmark::DoNotOptimize(p_memory);
    }
}

BENCHMARK(BM_AllocateUsingOpNew)->RangeMultiplier(2)->Range(4, 64);

BENCHMARK_MAIN();
