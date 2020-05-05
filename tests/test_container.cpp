#include "my_container.h"
#include "my_allocator.h"
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <algorithm>

TEST(MyContainer, basic_ops)
{
    OTUS::SimplisticContainer<int> c;
    c.push_back(1);
    c.push_back(2);
    ASSERT_EQ(2, c.size());
    ASSERT_EQ(1, c.front());
    ASSERT_EQ(2, c.back());
}

TEST(MyContainer, destructor)
{
    auto pc = new OTUS::SimplisticContainer<int, OTUS::logging_allocator_common_pool<int, 10>>();
    pc->push_back(1);
    auto alloc = pc->get_allocator();
    ASSERT_EQ(1, alloc.m_memory_pool_ptr->count());
    delete pc;
    ASSERT_EQ(0, alloc.m_memory_pool_ptr->count());
}

TEST(MyContainer, iterator)
{
    std::vector<int> exp_v{1, 2, 3};
    OTUS::SimplisticContainer<int> c;
    c.push_back(1);
    c.push_back(2);
    c.push_back(3);
    auto ite = exp_v.cbegin();
    for(auto it = c.begin(); it != c.end(); ++it, ++ite)
    {
        ASSERT_EQ(*ite, *it);
    }
}

TEST(MyContainer, stl_compat)
{
    OTUS::SimplisticContainer<int> c;
    c.push_back(1);
    c.push_back(2);
    c.push_back(3);
    std::vector<int> v;
    std::copy(c.begin(), c.end(), std::back_inserter(v));
    auto ite = v.cbegin();
    for(auto it = c.begin(); it != c.end(); ++it, ++ite)
    {
        ASSERT_EQ(*ite, *it);
    }
    
}
