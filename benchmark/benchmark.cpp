#include <benchmark/benchmark.h>

#include "vector.hpp"
#include "../test/DynamicInt.hpp"
#include <vector>

static void bench_vector(benchmark::State &state)
{
    low::vector<double> v{7, 8, 9};
    for (auto _ : state)
    {
        v.resize(10, 777);
        v.resize(2);
        v.resize(24,432);
        v.resize(10);
        v.resize(32);
    }
}
BENCHMARK(bench_vector);

static void bench_std_vector(benchmark::State &state)
{
    std::vector<double> v{7, 8, 9};
    for (auto _ : state)
    {
        v.resize(10, 777);
        v.resize(2);
        v.resize(24,432);
        v.resize(10);
        v.resize(32);
    }
}
BENCHMARK(bench_std_vector);
