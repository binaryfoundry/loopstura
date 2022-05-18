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

    bool Empty()
    {
        return head.load(std::memory_order_acquire) == tail.load(std::memory_order_relaxed);
    }

    T Read()
    {
        const size_t idx = to_index(tail.load(std::memory_order_relaxed));
        const T val = buffer[idx];
        tail.fetch_add(1);
        return val;
    }

    void Write(const T& val)
    {
        const size_t idx = to_index(head.load(std::memory_order_acquire));
        buffer[idx] = val;
        head.fetch_add(1);
    }

    size_t const Count() const
    {
        return head - tail;
    }

    size_t const Head() const
    {
        return head;
    }

    size_t const Tail() const
    {
        return tail;
    }

};
