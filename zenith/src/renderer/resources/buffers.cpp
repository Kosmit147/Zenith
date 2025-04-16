#include "zenith/renderer/resources/buffers.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/gl/buffer.hpp"
#include "zenith/log/logger.hpp"

namespace zth::buffers {

namespace {

std::unique_ptr<gl::Buffer> scratch_buffer = nullptr;

} // namespace

auto create() -> void
{
    ZTH_INTERNAL_TRACE("Creating buffers...");

    ZTH_ASSERT(scratch_buffer == nullptr);
    scratch_buffer = std::make_unique<gl::Buffer>(gl::Buffer::create_dynamic(gl::BufferUsage::dynamic_copy));

    ZTH_INTERNAL_TRACE("Buffers created.");
}

auto destroy() -> void
{
    ZTH_INTERNAL_TRACE("Destroying buffers...");

    ZTH_ASSERT(scratch_buffer != nullptr);
    scratch_buffer.reset();

    ZTH_INTERNAL_TRACE("Buffers destroyed.");
}

auto scratch() -> gl::Buffer&
{
    ZTH_ASSERT(scratch_buffer != nullptr);
    return *scratch_buffer;
}

} // namespace zth::buffers
