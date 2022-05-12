#pragma once

#include <array>
#include <atomic>

// static_assert(ATOMIC_INT_LOCK_FREE == 2, "atomic_int must be lock-free");

template <typename T, size_t S>
class RingBuffer
{
private:
    static inline size_t to_index(const size_t v)
    {
        return v % S;
    }

    std::array<T, S> buffer;

    std::atomic<size_t> head;
    std::atomic<size_t> tail;

public:
    RingBuffer()
    {
    }

    bool empty()
    {
        return head == tail;
    }

    T read()
    {
        uint64_t tail_index = to_index(tail);
        T val = buffer[tail_index];
        ++tail;
        return val;
    }

    void write(T& val)
    {
        buffer[to_index(head)] = val;
        ++head;
    }
};
