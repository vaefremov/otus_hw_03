#include "utilities.h"
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <algorithm>

TEST(Utilities, factorial)
{
    std::vector<int> args {-1, 0, 2, 5};
    std::vector<int> exp_v {1, 1, 2, 120};
    std::vector<int> v;
    std::transform(args.cbegin(), args.cend(), std::back_inserter(v), OTUS::factorial);
    ASSERT_EQ(exp_v, v);
}

TEST(Utilities, factorial_big)
{
    int exp_v1 = 362880; // 9!
    ASSERT_EQ(exp_v1, OTUS::factorial(9));
    ASSERT_THROW(OTUS::factorial(13), std::invalid_argument);
}
