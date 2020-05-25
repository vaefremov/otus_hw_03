#pragma once

#include <exception>
#include <my_container.h>
#include <my_allocator.h>

namespace OTUS
{

constexpr int factorial(int n) {
    if(n > 12)
    {
        throw std::invalid_argument("Factorial: argument is too large");
    }
    int res = 1;
    for (int i = 2; i <= n; i++)
    {
        res *= i;
    }
    
    // return (n < 1) ? 1 : factorial(n - 1) * n;
    return res;
}

}