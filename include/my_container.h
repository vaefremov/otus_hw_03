#pragma once
#include <memory>
#include <utility>

namespace OTUS
{

template<typename T>
struct Node
{
    Node(const T& f, Node<T>* s)
    {
        first = f;
        second = s;
    }
    T first;
    Node<T>* second;
};

template <typename T>
struct SimplisticContainerIterator {

    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;

    SimplisticContainerIterator() = default;

    explicit SimplisticContainerIterator(Node<T>* p_node) : m_node(p_node) {};
    SimplisticContainerIterator(const SimplisticContainerIterator&) = default;
    SimplisticContainerIterator& operator=(const SimplisticContainerIterator&) = default;

    reference operator*() const 
    {
        return m_node->first;
    }

    pointer operator->() const 
    {
        return &(m_node->first);
    }
    // Prefix ++
    SimplisticContainerIterator<T>& operator++() {
        if (m_node != nullptr) 
        {
            m_node = m_node->second;
        }
        return *this;
    }

    bool operator==(const SimplisticContainerIterator<T>& rhs) const {
        return m_node == rhs.m_node;
    }

    bool operator!=(const SimplisticContainerIterator<T>& rhs) const {
        return !(m_node == rhs.m_node);
    }

    Node<T>* m_node = nullptr;

};

template<typename T, typename A = std::allocator<T>>
class SimplisticContainer
{
    public:
    using NAllocator = typename A::template rebind <Node<T>>::other;

    SimplisticContainer() = default;
    ~SimplisticContainer()
    {
        while(m_head != nullptr)
        {
            auto next = m_head->second;
            m_alloc.destroy(m_head);
            m_alloc.deallocate(m_head, 1);
            m_head = next;
        }
    }
    size_t size()
    {
        return m_count;
    }
    void push_back(const T& el)
    {
        auto new_node = m_alloc.allocate(1);
        m_alloc.construct(new_node, el, nullptr);
        if(m_tail != nullptr)
        {
            m_tail->second = new_node;
        }
        m_tail = new_node;
        if(m_head == nullptr)
        {
            m_head = new_node;
        }
        ++m_count;
    }
    const T& front() const noexcept
    {
        return m_head->first;
    }

    const T& back() const noexcept
    {
        return m_tail->first;
    }
    const NAllocator& get_allocator() const noexcept
    {
        return m_alloc;
    }

    SimplisticContainerIterator<T> begin() const noexcept {
        return SimplisticContainerIterator<T>(m_head);
    }

    SimplisticContainerIterator<T> end() const noexcept {
        return SimplisticContainerIterator<T>();
    }

    private:

    size_t m_count = 0;
    Node<T>* m_head = nullptr;
    Node<T>* m_tail = nullptr;
    NAllocator m_alloc;
};


}