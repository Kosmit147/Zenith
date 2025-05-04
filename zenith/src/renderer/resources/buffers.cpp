#include "zenith/renderer/resources/buffers.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/gl/buffer.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/memory/managed.hpp"

namespace zth::buffers {

namespace {

UniquePtr<gl::Buffer> scratch_buffer = nullptr;
UniquePtr<gl::IndexBuffer> quads_ib = nullptr;

} // namespace

auto create() -> void
{
    ZTH_INTERNAL_TRACE("Creating buffers...");

    ZTH_ASSERT(scratch_buffer == nullptr);
    ZTH_ASSERT(quads_ib == nullptr);
    scratch_buffer = make_unique<gl::Buffer>(gl::Buffer::create_dynamic(gl::BufferUsage::dynamic_copy));
    quads_ib = make_unique<gl::IndexBuffer>(create_static_index_buffer_for_quads(quads_index_buffer_quads));

    ZTH_INTERNAL_TRACE("Buffers created.");
}

auto destroy() -> void
{
    ZTH_INTERNAL_TRACE("Destroying buffers...");

    ZTH_ASSERT(scratch_buffer != nullptr);
    ZTH_ASSERT(quads_ib != nullptr);
    scratch_buffer.free();
    quads_ib.free();

    ZTH_INTERNAL_TRACE("Buffers destroyed.");
}

auto scratch() -> gl::Buffer&
{
    return *scratch_buffer;
}

auto quads_index_buffer() -> gl::IndexBuffer&
{
    return *quads_ib;
}

} // namespace zth::buffers
