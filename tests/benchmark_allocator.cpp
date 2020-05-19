#include <benchmark/benchmark.h>
#include <iostream>
#include <memory>
#include <algorithm>

#include "my_allocator.h"

constexpr size_t n_allocations = 1000;


static void BM_std_alloc(benchmark::State& state)
{
    std::vector<int*> ptrs;
    ptrs.reserve(n_allocations);
    for(auto _: state)
    {
        auto stda = std::allocator<int>();
        for (size_t i = 0; i < n_allocations; i++)
        {
            ptrs.push_back(stda.allocate(1));
        }
        // std::copy(std::begin(ptrs), std::end(ptrs), std::ostream_iterator<int*>(std::cout, "\n"));
        for (auto p: ptrs)
        {
            stda.deallocate(p, 1);
        }
        ptrs.clear();
    }
}
BENCHMARK(BM_std_alloc);

static void BM_my_alloc(benchmark::State& state)
{
    std::vector<int*> ptrs;
    ptrs.reserve(n_allocations);
    for(auto _: state)
    {
        auto stda = OTUS::logging_allocator_common_pool<int, n_allocations>();
        for (size_t i = 0; i < n_allocations; i++)
        {
            ptrs.push_back(stda.allocate(1));
        }
        for (auto p: ptrs)
        {
            stda.deallocate(p, 1);
        }
        ptrs.clear();
    }
}
BENCHMARK(BM_my_alloc);



BENCHMARK_MAIN();
