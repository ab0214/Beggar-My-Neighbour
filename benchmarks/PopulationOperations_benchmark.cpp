#include <benchmark/benchmark.h>

#include "evolution/Population.h"
#include "evolution/PopulationOperations.h"

static void BM_SelectOnce(benchmark::State &state)
{
    Population pop(500);
    for (auto _ : state)
        PopulationOperations::truncationSelection(pop, 0.5);
}
BENCHMARK(BM_SelectOnce);

static void BM_SelectTwice(benchmark::State &state)
{
    Population pop(500);
    for (auto _ : state)
    {
        PopulationOperations::truncationSelection(pop, 0.5);
        PopulationOperations::truncationSelection(pop, 0.5);
    }
}
BENCHMARK(BM_SelectTwice);

static void BM_SelectFew(benchmark::State &state)
{
    Population pop(100);
    for (auto _ : state)
        PopulationOperations::truncationSelection(pop, 0.05);
}
BENCHMARK(BM_SelectFew);

static void BM_SelectMany(benchmark::State &state)
{
    Population pop(1000);
    for (auto _ : state)
        PopulationOperations::truncationSelection(pop, 0.75);
}
BENCHMARK(BM_SelectMany);

BENCHMARK_MAIN();