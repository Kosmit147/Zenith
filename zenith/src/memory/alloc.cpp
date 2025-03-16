#include "zenith/memory/alloc.hpp"

namespace zth::memory {

auto allocate(usize size_bytes) noexcept -> void*
{
    if (size_bytes == 0)
        return nullptr;

    auto ptr = std::malloc(size_bytes);
    ZTH_ASSERT(ptr != nullptr);

    return ptr;
}

} // namespace zth::memory
