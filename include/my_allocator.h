#pragma once

#include <memory>
#include <array>
#include <bitset>
#include <limits>

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

template<typename T, size_t SZ=DEFAULT_POOL_SZ>
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
    } 
    ~logging_allocator_common_pool() = default;
    logging_allocator_common_pool(const logging_allocator_common_pool<T, SZ>& other) = default;

    size_type max_size() const 
    {
        return std::numeric_limits<size_type>::max();
    }

    T* allocate(size_type n)
    {
        return m_memory_pool_ptr->allocate(n);
    }
    void deallocate(T* p, size_type n)
    {
        m_memory_pool_ptr->deallocate(p, n);
    }

    template<typename U, typename...Args>
    void construct(U* p, Args&& ...args)
    {
        new(p) U(std::forward<Args>(args)...);
    }
    template<typename U, typename...Args>
    void destroy(U* p)
    {
        p->~U();
    }

    std::shared_ptr<MemoryPool<T, SZ>> m_memory_pool_ptr;
};

template <typename T, typename U,  size_t SZ1=DEFAULT_POOL_SZ, size_t SZ2=DEFAULT_POOL_SZ>
bool operator==(logging_allocator_common_pool<T, SZ1> const&, logging_allocator_common_pool<U, SZ2> const&) noexcept
{
    // Should have used if constexpr here!
    if  (std::is_same<T, U>::value && SZ1 == SZ2)
    {
        return true;
    }
    return false;    
}

template <typename T, typename U,  size_t SZ1=DEFAULT_POOL_SZ, size_t SZ2=DEFAULT_POOL_SZ>
bool operator!=(logging_allocator_common_pool<T, SZ1> const& x, logging_allocator_common_pool<U, SZ2> const& y) noexcept
{
    return !(x == y);
}

}
