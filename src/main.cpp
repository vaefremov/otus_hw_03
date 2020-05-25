#include <iostream>
#include "utilities.h"
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

template<typename K, typename V, typename L, typename A>
static void print_map(const std::map<K, V, L, A>& m, std::ostream& out = std::cout)
{
    for(auto el: m)
    {
        out << el.first << " " << el.second << std::endl;
    }
}

template<typename T, typename A>
static void print_list(OTUS::SimplisticContainer<T, A>& c, std::ostream& out = std::cout)
{
    for(auto it = c.begin(); it != c.end(); ++it)
    {
        out << *it << std::endl;
    }
}

int main()
{
    std::map<int, int> m_stl;
    const int LIMIT = 10;
    for (size_t i = 0; i < LIMIT; i++)
    {
        m_stl[i] = OTUS::factorial(i);
    }
    print_map(m_stl);

    std::map<int, int, std::less<int>, OTUS::logging_allocator_common_pool<std::pair<const int, int>, 10>> m_my;
    for (size_t i = 0; i < LIMIT; i++)
    {
        m_my[i] = OTUS::factorial(i);
    }
    print_map(m_my);

    OTUS::SimplisticContainer<int> c_stl;
    for (size_t i = 0; i < LIMIT; i++)
    {
        c_stl.push_back(i);
    }

    OTUS::SimplisticContainer<int, OTUS::logging_allocator_common_pool<int, 10>> c_my;
    for (size_t i = 0; i < LIMIT; i++)
    {
        c_my.push_back(i);
    }

    print_list(c_stl);
    print_list(c_my);

    return 0;
}