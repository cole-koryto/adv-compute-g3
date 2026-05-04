#ifndef BINARY_HEAP_HPP
#define BINARY_HEAP_HPP

#include <vector>
#include <functional> // for std::less
#include <iostream>

template <typename T, typename Compare = std::less<T>>
class BinaryHeap
{
    std::vector<T> heap_;
    Compare cmp{};

    void sift_up(size_t i)
    {

        while (i > 0)
        {
            // auto &p = heap_[p_idx];
            // auto &c = heap_[i];

            size_t p_idx = parent(i);

            if (cmp(heap_[p_idx], heap_[i]))
            {
                // auto p = heap_[parent(i)];
                // auto c = heap_[i];
                std::swap(heap_[p_idx], heap_[i]);
                i = parent(i);
            }
            else
                break;
        }
    }

    void sift_down(size_t i)
    {

        size_t n = heap_.size();

        while (true)
        {
            size_t l = left(i);
            size_t r = right(i);
            size_t largest = i;

            if (l < n && cmp(heap_[largest], heap_[l]))
            {
                largest = l;
            }
            if (r < n && cmp(heap_[largest], heap_[r]))
            {
                largest = r;
            }

            if (largest == i)
            {
                break;
            }

            std::swap(heap_[largest], heap_[i]);
            i = largest;
        }
    }

public:
    void push(const T &value)
    {
        heap_.push_back(value);
        sift_up(heap_.size() - 1);
    }

    void pop()
    {
        //
        if (heap_.empty())
            return;

        std::swap(heap_[0], heap_.back());
        heap_.pop_back();
        if (!heap_.empty())
        {
            sift_down(0);
        }
    }

    const T &top() const
    {
        return heap_.front();
    }

    size_t size() const
    {
        return heap_.size();
    }

    bool empty() const
    {
        return heap_.empty();
    }

    // helpers
    static constexpr size_t parent(size_t i)
    {
        return (i - 1) / 2;
    }

    static constexpr size_t left(size_t i)
    {
        return 2 * i + 1;
    }

    static constexpr size_t right(size_t i)
    {
        return 2 * i + 2;
    }

    void reserve(size_t s)
    {
        heap_.reserve(s);
    }

    // warning, will fail for anything other than an order.
    void print()
    {
        if (heap_.empty())
        {
            std::cout << "[]\n";
            return;
        }
        std::cout << "[";
        for (auto i = heap_.begin(); i != heap_.end() - 1; i++)
        {
            std::cout << i->price << ", ";
        }
        std::cout << heap_.back().price;
        std::cout << "]" << '\n';
    }
};

#endif // BINARY_HEAP_HPP