#include "zenith/system/temporary_storage.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/util/number.hpp"

namespace zth {

memory::Buffer<> TemporaryStorage::_buffer{ initial_capacity };
Vector<UniquePtr<byte[]>> TemporaryStorage::_overflow_allocations;

auto TemporaryStorage::init() -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Initializing temporary storage...");
    reset();
    ZTH_INTERNAL_TRACE("Temporary storage initialized.");
    return {};
}

auto TemporaryStorage::start_frame() -> void
{
    reset();
}

auto TemporaryStorage::shut_down() -> void
{
    ZTH_INTERNAL_TRACE("Shutting down temporary storage...");
    reset();
    _buffer.free();
    _buffer_ptr = nullptr;
    ZTH_INTERNAL_TRACE("Temporary storage shut down.");
}

auto TemporaryStorage::reset() -> void
{
    _buffer_ptr = begin();
    free_overflow_allocations();
}

auto TemporaryStorage::reset_with_new_capacity(usize new_capacity) -> void
{
    _buffer.free();
    _buffer.resize(new_capacity);
    reset();
}

auto TemporaryStorage::allocate(usize size_bytes, usize alignment) -> void*
{
    ZTH_ASSERT(is_power_of_2(alignment));

    if (size_bytes == 0) [[unlikely]]
        return nullptr;

    memory::align(_buffer_ptr, alignment);
    return allocate_unaligned(size_bytes);
}

auto TemporaryStorage::allocate_unaligned(usize size_bytes) -> void*
{
    // Handles the case when _buffer_ptr is nullptr as well.

    if (size_bytes == 0) [[unlikely]]
        return nullptr;

    auto result = _buffer_ptr;
    _buffer_ptr += size_bytes;

    // Make sure _buffer_ptr didn't overflow.
    ZTH_ASSERT(_buffer_ptr >= begin());

    if (_buffer_ptr > end()) [[unlikely]]
    {
        _buffer_ptr = end();
        return allocate_if_overflowed(size_bytes);
    }

    return result;
}

auto TemporaryStorage::memory_left() -> usize
{
    return std::distance(_buffer_ptr, end());
}

auto TemporaryStorage::memory_used() -> usize
{
    return std::distance(begin(), _buffer_ptr);
}

auto TemporaryStorage::allocate_if_overflowed(usize size_bytes) -> void*
{
    _overflow_allocations.push_back(make_unique_for_overwrite<byte[]>(size_bytes));
    return _overflow_allocations.back().get();
}

auto TemporaryStorage::free_overflow_allocations() -> void
{
    _overflow_allocations.clear();
    _overflow_allocations.shrink_to_fit();
}

} // namespace zth
