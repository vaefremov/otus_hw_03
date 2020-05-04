#pragma once

#include <memory>
#include <array>
#include <bitset>

namespace OTUS
{

constexpr size_t DEFAULT_POOL_SZ = 10;

template<typename T, size_t SZ=DEFAULT_POOL_SZ>
class MemoryPool
{
    public:

    MemoryPool() = default;
    ~MemoryPool() = default;

    T* allocate(size_t n) 
    {   
        if(n < 1)
        {
            throw std::invalid_argument("<1");
        }
        if (n > SZ - m_pool.size())
        {
            throw std::bad_alloc();
        }
        for(size_t i = 0; i < m_assigned.size()-n+1; ++i)
        {
            size_t j = 0;
            // Find n successive free bits
            for(; j<n && !m_assigned[i+j]; ++j);
            if (j == n) {
                // n free bits found, mark them as assigned
                for(size_t k=i; k < i+n; m_assigned[k++] = true);
                return reinterpret_cast<T*>(&m_pool[i*sizeof(T)]);
            }
            i += j;
        }
        throw std::bad_alloc();
    }

    void deallocate(T const* p, size_t n)
    {
        if(n < 1)
        {
            throw std::invalid_argument("<1");
        }
        // We assume that the pointer really belongs to the current memory pool
        size_t ind = p - reinterpret_cast<T*>(&m_pool[0]);
        if(ind >= m_pool.size())
        {
            throw std::invalid_argument("invalid pointer (probably not from this pool)");
        }
        // Mark the corresponding memory blocks as free
        for (size_t i = ind; i < ind + n && i < m_assigned.size(); m_assigned[i++] = false);
    }

    size_t count() const
    {
        return m_assigned.count();
    }

    size_t n_free() const
    {
        return SZ-m_assigned.count();
    }

    private:
    std::bitset<SZ> m_assigned;
    std::array<char, SZ*sizeof(T)> m_pool;
};

// 
template<typename T, size_t SZ>
std::shared_ptr<MemoryPool<T, SZ>> getPool()
{
    static std::shared_ptr<MemoryPool<T, SZ>> shmp = std::make_shared<MemoryPool<T, SZ>>();
    return shmp;
}

template<typename T, int SZ=DEFAULT_POOL_SZ>
struct logging_allocator_common_pool
{
    using value_type = T;
    using size_type = std::size_t;

    template<typename U>
    struct rebind
    {
        using other = logging_allocator_common_pool<U, SZ>;
    };

    logging_allocator_common_pool() 
    {
        m_memory_pool_ptr = getPool<T, SZ>();
        // std::cout << "Allocator constructor with " << m_memory_pool_ptr->n_free() << " " << __PRETTY_FUNCTION__ << std::endl;
    } 
    ~logging_allocator_common_pool() {};
    template<typename U>
    logging_allocator_common_pool(const logging_allocator_common_pool<U>& other) = delete;

    size_type max_size() const 
    {
        return std::numeric_limits<size_type>::max();
    }

    T* allocate(size_type n)
    {
        auto p = m_memory_pool_ptr->allocate(n);
        // std::cout << GREEN << __PRETTY_FUNCTION__ << " n " << n << " n_free " << m_memory_pool_ptr->n_free() << " " << p << WHITE << std::endl;
        return p;
    }
    void deallocate(T* p, size_type n)
    {
        // std::cout << RED << __PRETTY_FUNCTION__ << " " << p << " " << n <<  WHITE << std::endl;
        m_memory_pool_ptr->deallocate(p, n);
    }

    template<typename U, typename...Args>
    void construct(U* p, Args&& ...args)
    {
        // std::cout << BLUE << __PRETTY_FUNCTION__ << WHITE << std::endl;
        new(p) U(std::forward<Args>(args)...);
    }
    template<typename U, typename...Args>
    void destroy(U* p)
    {
        // std::cout << YELLOW << __PRETTY_FUNCTION__ << WHITE << std::endl;
        p->~U();
    }

    std::shared_ptr<MemoryPool<T, SZ>> m_memory_pool_ptr;
};

}
