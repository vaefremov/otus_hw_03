#include "my_allocator.h"
#include <gtest/gtest.h>
#include <iostream>
#include <map>

TEST(MemoryPool, shared_pool)
{
    // Making sure the objects are allocated in the same pool
    auto mp1 = OTUS::getPool<std::pair<int, int>, 10>();
    auto mp2 = OTUS::getPool<std::pair<int, int>, 10>();
    mp1->allocate(1);
    mp1->allocate(2);
    ASSERT_EQ(mp1->count(), mp2->count());
    ASSERT_EQ(mp1, mp2);
    auto mp3 = OTUS::getPool<std::pair<int, int>, 20>();
    ASSERT_NE(mp1->count(), mp3->count());
}

TEST(MyAllocator, allocating_vector)
{
   std::vector<int, OTUS::logging_allocator_common_pool<int, 10>> v;
   std::vector<int> exp_v{2, 3, 4};
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    for (size_t i = 0; i < v.size(); i++)
    {
        ASSERT_EQ(exp_v.at(i), v.at(i));
    }
    // We've got only limited space in allocator of the given type
    std::vector<int, OTUS::logging_allocator_common_pool<int, 10>> v2{1, 2, 3};
    ASSERT_THROW(v2.push_back(42), std::bad_alloc);
}

TEST(MyAllocator, allocating_map)
{
   std::map<int, int, std::less<int>,OTUS::logging_allocator_common_pool<std::pair<const int, int>, 10>> m;
    m[1] = 2;
    m[2] = 4;
    m[3] = 3;
    std::map<int, int> exp_m{{1, 2}, {2, 4}, {3, 3}};
    for (auto& e: exp_m)
    {
        ASSERT_EQ(e.second, m[e.first]);
    }
}

TEST(MyAllocator, equality)
{
    auto al1 = OTUS::logging_allocator_common_pool<int, 42>();
    auto al2 = OTUS::logging_allocator_common_pool<int, 42>();
    ASSERT_TRUE(al1 == al2);
    auto al_diff = OTUS::logging_allocator_common_pool<int, 10>();
    ASSERT_FALSE(al1 == al_diff);
    ASSERT_TRUE(al1 != al_diff);
    auto al_diff1 = OTUS::logging_allocator_common_pool<std::pair<int, int>, 10>();
    ASSERT_FALSE(al_diff1 == al_diff);
    ASSERT_TRUE(al_diff1 != al_diff);
}