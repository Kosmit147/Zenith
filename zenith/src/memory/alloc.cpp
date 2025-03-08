#include "zenith/memory/alloc.hpp"

#include "zenith/core/assert.hpp"

namespace zth::memory {

auto allocate(usize size_bytes) noexcept -> void*
{
    if (size_bytes == 0)
        return nullptr;

    auto ptr = std::malloc(size_bytes);
    ZTH_ASSERT(ptr != nullptr);

    return ptr;
}

template<> auto reallocate(void*& ptr, usize new_size_bytes) noexcept -> void
{
    if (new_size_bytes == 0)
    {
        deallocate(ptr);
        return;
    }

    ptr = std::realloc(ptr, new_size_bytes);
    ZTH_ASSERT(ptr != nullptr);
}

template<> auto reallocate(void*& ptr, usize new_size_bytes, usize buffer_size_bytes) noexcept -> void
{
    if (new_size_bytes == 0)
    {
        deallocate(ptr);
        return;
    }

    // @speed: maybe using std::realloc would be faster
    // the rationale for what we're doing right now is that buffer_size_bytes could potentially be a lot smaller than
    // the old size of the allocated block, and therefore we avoid unnecessarily copying memory
    auto old_ptr = ptr;
    ptr = allocate(new_size_bytes);
    std::memcpy(ptr, old_ptr, std::min(new_size_bytes, buffer_size_bytes));
    deallocate(old_ptr);
}

template<> auto deallocate(void*& ptr) noexcept -> void
{
    std::free(ptr);
    ptr = nullptr;
}

} // namespace zth::memory
