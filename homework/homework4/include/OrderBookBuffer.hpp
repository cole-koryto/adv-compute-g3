#pragma once

#include <cstddef>
#include <cstring>
#include <iostream>
#include <mutex>
#include <new>
#include <stdexcept>
#include <type_traits>

// =====================
// Allocator Policies
// =====================

template <typename T>
class HeapAllocator {
public:
    T* allocate(std::size_t n) {
        return new T[n];
    }

    void deallocate(T* ptr) {
        delete[] ptr;
    }
};

template <typename T, std::size_t N>
class StackAllocator {
private:
    alignas(T) unsigned char buffer_[N * sizeof(T)];
    std::size_t offset_ = 0;

public:
    T* allocate(std::size_t n) {
        if (offset_ + n > N) {
            throw std::bad_alloc();
        }

        T* ptr = reinterpret_cast<T*>(buffer_ + offset_ * sizeof(T));
        offset_ += n;
        return ptr;
    }

    void deallocate(T*) {
        // no-op: stack allocator frees all memory when allocator dies
    }

    void reset() {
        offset_ = 0;
    }
};

template <typename T>
class ZeroInitAllocator {
public:
    T* allocate(std::size_t n) {
        T* ptr = new T[n]{};
        return ptr;
    }

    void deallocate(T* ptr) {
        delete[] ptr;
    }
};

// =====================
// Threading Policies
// =====================

class NoLock {
public:
    void lock() {}
    void unlock() {}
};

class MutexLock {
private:
    std::mutex mtx_;

public:
    void lock() {
        mtx_.lock();
    }

    void unlock() {
        mtx_.unlock();
    }
};

// =====================
// RAII Lock Guard
// =====================

template <typename ThreadingPolicy>
class LockGuard {
private:
    ThreadingPolicy& lock_;

public:
    explicit LockGuard(ThreadingPolicy& lock) : lock_(lock) {
        lock_.lock();
    }

    ~LockGuard() {
        lock_.unlock();
    }
};

// =====================
// OrderBookBuffer
// =====================

template <typename T, typename AllocatorPolicy, typename ThreadingPolicy>
class OrderBookBuffer {
private:
    T* orders_;
    std::size_t capacity_;
    std::size_t size_;

    AllocatorPolicy allocator_;
    ThreadingPolicy lock_;

public:
    explicit OrderBookBuffer(std::size_t capacity)
        : orders_(nullptr), capacity_(capacity), size_(0)
    {
        orders_ = allocator_.allocate(capacity_);
    }

    ~OrderBookBuffer() {
        allocator_.deallocate(orders_);
    }

    void add_order(const T& order) {
        LockGuard<ThreadingPolicy> guard(lock_);

        if (size_ >= capacity_) {
            throw std::out_of_range("OrderBookBuffer is full");
        }

        orders_[size_] = order;
        ++size_;
    }

    void print_orders() {
        LockGuard<ThreadingPolicy> guard(lock_);

        for (std::size_t i = 0; i < size_; ++i) {
            std::cout << orders_[i] << '\n';
        }
    }

    std::size_t size() const {
        return size_;
    }

    std::size_t capacity() const {
        return capacity_;
    }

    T& operator[](std::size_t i) {
        return orders_[i];
    }

    const T& operator[](std::size_t i) const {
        return orders_[i];
    }
};