#include "zenith/memory/alloc.hpp"

#include "zenith/core/assert.hpp"

namespace zth {

auto allocate(usize size_bytes) noexcept -> void*
{
    ZTH_ASSERT(size_bytes != 0);

    auto ptr = std::malloc(size_bytes);
    ZTH_ASSERT(ptr != nullptr);

    return ptr;
}

template<> auto reallocate(void*& ptr, usize new_size_bytes) noexcept -> void
{
    ZTH_ASSERT(new_size_bytes != 0);

    ptr = std::realloc(ptr, new_size_bytes);
    ZTH_ASSERT(ptr != nullptr);
}

template<> auto reallocate<void>(void*& ptr, usize new_size_bytes, usize buffer_size_bytes) noexcept -> void
{
    ZTH_ASSERT(new_size_bytes != 0);

    // @speed: maybe using std::realloc would be faster
    // the rationale for what we're doing right now is that buffer_size_bytes could potentially be a lot smaller than
    // the old size of the allocated block, and therefore we avoid unnecessarily copying memory
    auto old_ptr = ptr;
    ptr = allocate(new_size_bytes);
    std::memcpy(ptr, old_ptr, std::min(new_size_bytes, buffer_size_bytes));
    free(old_ptr);
}

auto free(void* ptr) noexcept -> void
{
    std::free(ptr);
}

} // namespace zth
