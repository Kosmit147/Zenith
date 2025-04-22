#include "zenith/renderer/resources/buffers.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/gl/buffer.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/memory/managed.hpp"

namespace zth::buffers {

namespace {

UniquePtr<gl::Buffer> scratch_buffer = nullptr;

} // namespace

auto create() -> void
{
    ZTH_INTERNAL_TRACE("Creating buffers...");

    ZTH_ASSERT(scratch_buffer == nullptr);
    scratch_buffer = make_unique<gl::Buffer>(gl::Buffer::create_dynamic(gl::BufferUsage::dynamic_copy));

    ZTH_INTERNAL_TRACE("Buffers created.");
}

auto destroy() -> void
{
    ZTH_INTERNAL_TRACE("Destroying buffers...");

    ZTH_ASSERT(scratch_buffer != nullptr);
    scratch_buffer.free();

    ZTH_INTERNAL_TRACE("Buffers destroyed.");
}

auto scratch() -> gl::Buffer&
{
    return *scratch_buffer;
}

} // namespace zth::buffers
