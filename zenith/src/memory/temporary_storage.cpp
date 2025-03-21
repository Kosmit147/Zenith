#include "zenith/memory/temporary_storage.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/math/number.hpp"

namespace zth {

memory::Buffer TemporaryStorage::_buffer{ initial_capacity };
Vector<std::unique_ptr<byte[]>> TemporaryStorage::_overflow_allocations;

auto TemporaryStorage::init() -> void
{
    ZTH_CORE_INFO("Initializing temporary storage...");

    reset();

    ZTH_CORE_INFO("Temporary storage initialized.");
}

auto TemporaryStorage::update() -> void
{
    reset();
}

auto TemporaryStorage::shut_down() -> void
{
    ZTH_CORE_INFO("Shutting down temporary storage...");

    reset();
    _buffer.free();

    ZTH_CORE_INFO("Temporary storage shut down.");
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
    ZTH_ASSERT(_buffer_ptr != nullptr);
    ZTH_ASSERT(math::is_power_of_2(alignment));

    if (size_bytes == 0) [[unlikely]]
        return nullptr;

    memory::align(_buffer_ptr, alignment);
    return allocate_unaligned(size_bytes);
}

auto TemporaryStorage::allocate_unaligned(usize size_bytes) -> void*
{
    ZTH_ASSERT(_buffer_ptr != nullptr);

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

auto TemporaryStorage::space_left() -> usize
{
    return std::distance(_buffer_ptr, end());
}

auto TemporaryStorage::space_used() -> usize
{
    return std::distance(begin(), _buffer_ptr);
}

auto TemporaryStorage::allocate_if_overflowed(usize size_bytes) -> void*
{
    _overflow_allocations.push_back(std::make_unique_for_overwrite<byte[]>(size_bytes));
    return _overflow_allocations.back().get();
}

auto TemporaryStorage::free_overflow_allocations() -> void
{
    _overflow_allocations.clear();
    _overflow_allocations.shrink_to_fit();
}

} // namespace zth
