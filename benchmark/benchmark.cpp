#include <benchmark/benchmark.h>

#include "vector.hpp"

static void bench_vector(benchmark::State &state)
{
    for (auto _ : state)
    {
        low::vector v{7, 8, 9};
    }
}
BENCHMARK(bench_vector);
