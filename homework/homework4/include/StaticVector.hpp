#pragma once
template <class T, size_t N>
class StaticVector
{
private:
    T array[N];
    int head_index;

public:
    StaticVector()
    {
        array = new T[N];
        head_index = 0;
    }

    ~StaticVector()
    {
        delete[] array;
    }

    void push_back(T element)
    {
        array[head_index] = element;
        head_index++;
    }

    T& operator[](int index)
    {
        return array[index];
    }

    int size()
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
