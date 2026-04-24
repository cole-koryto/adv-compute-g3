#pragma once
#include <cstddef>

template <class T, size_t N>
class StaticVector
{
private:
    T array[N]{};
    size_t head_index;

public:
    StaticVector() : head_index(0) {}

    void push_back(T element)
    {
        if (head_index < N) {
            array[head_index] = element;
            ++head_index;
        }
    }

    T& operator[](int index)
    {
        return array[index];
    }

    size_t size() const
    {
        return head_index;
    }

    T* begin()
    {
        return array;
    }

    T* end()
    {
        return array + head_index;
    }
};
