#include "zenith/memory/alloc.hpp"

namespace zth::memory {

auto allocate(usize size_bytes) noexcept -> void*
{
    if (size_bytes == 0) [[unlikely]]
        return nullptr;

    auto ptr = std::malloc(size_bytes);
    ZTH_ASSERT(ptr != nullptr);

    return ptr;
}

auto allocate_zeroed(usize size_bytes) noexcept -> void*
{
    auto ptr = allocate(size_bytes);
    std::memset(ptr, 0, size_bytes);
    return ptr;
}

} // namespace zth::memory
