#pragma once

#include <exception>
#include <my_container.h>
#include <my_allocator.h>

namespace OTUS
{

inline int factorial(int n) {
    if(n > 12)
    {
        throw std::invalid_argument("Factorial: argument is too large");
    }
    return (n < 1) ? 1 : factorial(n - 1) * n;
}

}