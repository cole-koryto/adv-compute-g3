#include <iostream>
#include <iomanip>
#include <robin_hood_map.hpp>
#include <array>
#include <string>
#include <unordered_map>
#include <chrono>

int main()
{
    int size = 2 << 20;
    RobinHoodMap<int> map(size << 1);
    std::unordered_map<std::string, int> umap;
    umap.reserve(size);

    std::vector<std::string> keys;
    for (int i = 0; i < size; i++)
    {
        keys.push_back(std::to_string(i));
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < size; i++)
    {
        map.insert(keys[i], i);
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < size; i++)
    {
        umap[keys[i]] = i;
    }

    auto t3 = std::chrono::high_resolution_clock::now();

    // force compiler to not optimize this loop away
    volatile size_t sink = 0;
    auto t4 = std::chrono::high_resolution_clock::now();
    for (auto &k : keys)
    {
        auto *v = map.find(k);
        if (v)
            sink += *v;
    }
    auto t5 = std::chrono::high_resolution_clock::now();

    sink = 0;
    auto t6 = std::chrono::high_resolution_clock::now();
    for (auto &k : keys)
    {
        auto it = umap.find(k);
        if (it != umap.end())
            sink += it->second;
    }
    auto t7 = std::chrono::high_resolution_clock::now();

    std::cout << "RobinHoood: " << std::chrono::duration<double>(t2 - t1).count() << " s" << std::endl;
    std::cout << "lookup time: " << std::chrono::duration<double>(t5 - t4).count() << " s" << std::endl;
    std::cout << "UnorderedMap: " << std::chrono::duration<double>(t3 - t2).count() << " s" << std::endl;
    std::cout << "lookup time: " << std::chrono::duration<double>(t7 - t6).count() << " s" << std::endl;
}