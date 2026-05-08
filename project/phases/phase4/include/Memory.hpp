#pragma once

#include <memory>
#include <memory_resource>
#include <new>
#include <utility>

namespace hft::phase4
{
template <typename T>
struct PmrDeleter
{
    std::pmr::memory_resource *resource = nullptr;

    void operator()(T *ptr) const noexcept
    {
        if (!ptr)
            return;
        std::pmr::polymorphic_allocator<T> alloc(resource);
        std::destroy_at(ptr);
        alloc.deallocate(ptr, 1);
    }
};

template <typename T, typename... Args>
std::unique_ptr<T, PmrDeleter<T>> pmr_make_unique(std::pmr::memory_resource *resource, Args &&...args)
{
    std::pmr::polymorphic_allocator<T> alloc(resource);
    T *ptr = alloc.allocate(1);
    try
    {
        std::construct_at(ptr, std::forward<Args>(args)...);
    }
    catch (...)
    {
        alloc.deallocate(ptr, 1);
        throw;
    }
    return std::unique_ptr<T, PmrDeleter<T>>(ptr, PmrDeleter<T>{resource});
}
} // namespace hft::phase4

