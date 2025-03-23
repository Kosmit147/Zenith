#pragma once

#include <glad/glad.h>

#include <ranges>

#include "zenith/core/typedefs.hpp"
#include "zenith/util/macros.hpp"
#include "zenith/util/optional.hpp"

// @cleanup: Buffers implementation.

namespace zth::gl {

enum class BufferAccessFrequency : u8
{
    // @volatile: These numbers are chosen for converting BufferUsage to an OpenGL value.

    Stream = 0,  // The data store contents will be modified once and used at most a few times.
    Static = 4,  // The data store contents will be modified once and used many times.
    Dynamic = 8, // The data store contents will be modified repeatedly and used many times.
};

enum class BufferAccessType : u8
{
    // @volatile: These numbers are chosen for converting BufferUsage to an OpenGL value.

    Draw = 0, // The data store contents are modified by the application, and used as the source for GL drawing and
              // image specification commands. The user will be writing data to the buffer, but will not read it.
    Read = 1, // The data store contents are modified by reading data from the GL, and used to return that data when
              // queried by the application. The user will not be writing data to the buffer, but will be reading it
              // back.
    Copy = 2, // The data store contents are modified by reading data from the GL, and used as the source for GL drawing
              // and image specification commands. The user will be neither writing not reading the data.
};

struct BufferUsage
{
    BufferAccessFrequency access_frequency;
    BufferAccessType access_type;

    static constinit const BufferUsage stream_draw;
    static constinit const BufferUsage static_draw;
    static constinit const BufferUsage dynamic_draw;

    static constinit const BufferUsage stream_read;
    static constinit const BufferUsage static_read;
    static constinit const BufferUsage dynamic_read;

    static constinit const BufferUsage stream_copy;
    static constinit const BufferUsage static_copy;
    static constinit const BufferUsage dynamic_copy;
};

enum class BufferState : u8
{
    Uninitialized,
    InitializedStatic,  // Buffer cannot be reinitialized, size cannot be changed and cannot be 0.
    InitializedDynamic, // Buffer can be reinitialized, size can be changed.
};

// --------------------------- Buffer ---------------------------

class Buffer
{
public:
    using BufferId = GLuint;

public:
    explicit Buffer();

    [[nodiscard]] static auto create_static_with_size(u32 size_bytes) -> Buffer;
    [[nodiscard]] static auto create_static_with_data(const void* data, u32 data_size_bytes) -> Buffer;
    [[nodiscard]] static auto create_static_with_data(auto&& data) -> Buffer;
    [[nodiscard]] static auto create_static_with_data(std::ranges::contiguous_range auto&& data) -> Buffer;

    [[nodiscard]] static auto create_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> Buffer;
    [[nodiscard]] static auto create_dynamic_with_size(u32 size_bytes, BufferUsage usage = BufferUsage::dynamic_draw)
        -> Buffer;
    [[nodiscard]] static auto create_dynamic_with_data(const void* data, u32 data_size_bytes,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> Buffer;
    [[nodiscard]] static auto create_dynamic_with_data(auto&& data, BufferUsage usage = BufferUsage::dynamic_draw)
        -> Buffer;
    [[nodiscard]] static auto create_dynamic_with_data(std::ranges::contiguous_range auto&& data,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> Buffer;

    ZTH_NO_COPY(Buffer)

    Buffer(Buffer&& other) noexcept;
    auto operator=(Buffer&& other) noexcept -> Buffer&;

    ~Buffer();

    auto init_static_with_size(u32 size_bytes) -> void;
    auto init_static_with_data(const void* data, u32 data_size_bytes) -> void;
    auto init_static_with_data(auto&& data) -> void;
    auto init_static_with_data(std::ranges::contiguous_range auto&& data) -> void;

    auto init_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic_with_size(u32 size_bytes, BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic_with_data(const void* data, u32 data_size_bytes, BufferUsage usage = BufferUsage::dynamic_draw)
        -> void;
    auto init_dynamic_with_data(auto&& data, BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic_with_data(std::ranges::contiguous_range auto&& data,
                                BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    // Returns the number of bytes written.
    auto buffer_data(const void* data, u32 data_size_bytes, u32 offset = 0) -> u32;
    // Returns the number of bytes written.
    auto buffer_data(auto&& data, u32 offset = 0) -> u32;
    // Returns the number of bytes written.
    auto buffer_data(std::ranges::contiguous_range auto&& data, u32 offset = 0) -> u32;

    auto reserve(u32 min_size_bytes) -> void;
    auto resize(u32 size_bytes) -> void;

    [[nodiscard]] auto native_handle() const { return _id; }
    [[nodiscard]] auto size_bytes() const { return _size_bytes; }
    [[nodiscard]] auto is_static() const { return _state == BufferState::InitializedStatic; }
    [[nodiscard]] auto is_dynamic() const { return _state == BufferState::InitializedDynamic; }
    [[nodiscard]] auto is_initialized() const { return _state != BufferState::Uninitialized; }

private:
    BufferId _id = GL_NONE;
    u32 _size_bytes = 0;
    BufferState _state = BufferState::Uninitialized;
    Optional<BufferUsage> _usage = nil;

private:
    auto create() noexcept -> void;
    auto destroy() noexcept -> void;

    auto reallocate(u32 new_size_bytes) -> void;

    [[nodiscard]] static auto calculate_growth(u32 old_size_bytes) -> u32;
};

// --------------------------- VertexBuffer ---------------------------

class VertexBuffer
{
public:
    explicit VertexBuffer() = default;

    [[nodiscard]] static auto create_static_with_size(u32 size_bytes) -> VertexBuffer;
    [[nodiscard]] static auto create_static_with_data(const void* data, u32 data_size_bytes) -> VertexBuffer;
    [[nodiscard]] static auto create_static_with_data(std::ranges::contiguous_range auto&& data) -> VertexBuffer;

    [[nodiscard]] static auto create_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> VertexBuffer;
    [[nodiscard]] static auto create_dynamic_with_size(u32 size_bytes, BufferUsage usage = BufferUsage::dynamic_draw)
        -> VertexBuffer;
    [[nodiscard]] static auto create_dynamic_with_data(const void* data, u32 data_size_bytes,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> VertexBuffer;
    [[nodiscard]] static auto create_dynamic_with_data(std::ranges::contiguous_range auto&& data,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> VertexBuffer;

    ZTH_NO_COPY(VertexBuffer)

    VertexBuffer(VertexBuffer&& other) noexcept;
    auto operator=(VertexBuffer&& other) noexcept -> VertexBuffer&;

    ~VertexBuffer() = default;

    auto init_static_with_size(u32 size_bytes) -> void;
    auto init_static_with_data(const void* data, u32 data_size_bytes) -> void;
    auto init_static_with_data(std::ranges::contiguous_range auto&& data) -> void;

    auto init_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic_with_size(u32 size_bytes, BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic_with_data(const void* data, u32 data_size_bytes, BufferUsage usage = BufferUsage::dynamic_draw)
        -> void;
    auto init_dynamic_with_data(std::ranges::contiguous_range auto&& data,
                                BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto buffer_data(const void* data, u32 data_size_bytes, u32 offset = 0) -> u32;
    auto buffer_data(std::ranges::contiguous_range auto&& data, u32 offset = 0) -> u32;

    auto bind() const -> void;
    static auto unbind() -> void;

    auto set_stride(u32 stride_bytes) -> void;

    [[nodiscard]] auto native_handle() const { return _buffer.native_handle(); }
    [[nodiscard]] auto size_bytes() const { return _buffer.size_bytes(); }
    [[nodiscard]] auto is_static() const { return _buffer.is_static(); }
    [[nodiscard]] auto is_dynamic() const { return _buffer.is_dynamic(); }
    [[nodiscard]] auto is_initialized() const { return _buffer.is_initialized(); }
    [[nodiscard]] auto stride() const { return _stride_bytes; }

private:
    Buffer _buffer;
    u32 _stride_bytes = 0;
};

// --------------------------- IndexBuffer ---------------------------

class IndexBuffer
{
public:
    explicit IndexBuffer() = default;

    [[nodiscard]] static auto create_static_with_size(u32 size_bytes) -> IndexBuffer;
    [[nodiscard]] static auto create_static_with_data(const void* data, u32 data_size_bytes) -> IndexBuffer;
    [[nodiscard]] static auto create_static_with_data(std::ranges::contiguous_range auto&& data) -> IndexBuffer;

    [[nodiscard]] static auto create_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> IndexBuffer;
    [[nodiscard]] static auto create_dynamic_with_size(u32 size_bytes, BufferUsage usage = BufferUsage::dynamic_draw)
        -> IndexBuffer;
    [[nodiscard]] static auto create_dynamic_with_data(const void* data, u32 data_size_bytes,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> IndexBuffer;
    [[nodiscard]] static auto create_dynamic_with_data(std::ranges::contiguous_range auto&& data,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> IndexBuffer;

    ZTH_NO_COPY(IndexBuffer)

    IndexBuffer(IndexBuffer&& other) noexcept;
    auto operator=(IndexBuffer&& other) noexcept -> IndexBuffer&;

    ~IndexBuffer() = default;

    auto init_static_with_size(u32 size_bytes) -> void;
    auto init_static_with_data(const void* data, u32 data_size_bytes) -> void;
    auto init_static_with_data(std::ranges::contiguous_range auto&& data) -> void;

    auto init_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic_with_size(u32 size_bytes, BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic_with_data(const void* data, u32 data_size_bytes, BufferUsage usage = BufferUsage::dynamic_draw)
        -> void;
    auto init_dynamic_with_data(std::ranges::contiguous_range auto&& data,
                                BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto buffer_data(const void* data, u32 data_size_bytes, u32 offset = 0) -> u32;
    auto buffer_data(std::ranges::contiguous_range auto&& data, u32 offset = 0) -> u32;

    auto bind() const -> void;
    static auto unbind() -> void;

    template<typename T> auto set_index_type() -> void;
    auto set_index_type(GLenum index_type) -> void;
    auto set_size(u32 size) -> void;

    [[nodiscard]] auto native_handle() const { return _buffer.native_handle(); }
    [[nodiscard]] auto size_bytes() const { return _buffer.size_bytes(); }
    [[nodiscard]] auto is_static() const { return _buffer.is_static(); }
    [[nodiscard]] auto is_dynamic() const { return _buffer.is_dynamic(); }
    [[nodiscard]] auto is_initialized() const { return _buffer.is_initialized(); }
    [[nodiscard]] auto index_type() const { return _index_type; }
    [[nodiscard]] auto size() const { return _size; }

private:
    Buffer _buffer;
    GLenum _index_type = GL_NONE;
    u32 _size = 0;
};

// --------------------------- InstanceBuffer ---------------------------

class InstanceBuffer : public VertexBuffer
{
public:
    explicit InstanceBuffer() = default;

    [[nodiscard]] static auto create_static_with_size(u32 size_bytes) -> InstanceBuffer;
    [[nodiscard]] static auto create_static_with_data(const void* data, u32 data_size_bytes) -> InstanceBuffer;
    [[nodiscard]] static auto create_static_with_data(std::ranges::contiguous_range auto&& data) -> InstanceBuffer;

    [[nodiscard]] static auto create_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> InstanceBuffer;
    [[nodiscard]] static auto create_dynamic_with_size(u32 size_bytes, BufferUsage usage = BufferUsage::dynamic_draw)
        -> InstanceBuffer;
    [[nodiscard]] static auto create_dynamic_with_data(const void* data, u32 data_size_bytes,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> InstanceBuffer;
    [[nodiscard]] static auto create_dynamic_with_data(std::ranges::contiguous_range auto&& data,
                                                       BufferUsage usage = BufferUsage::dynamic_draw) -> InstanceBuffer;

    ZTH_NO_COPY(InstanceBuffer)
    ZTH_DEFAULT_MOVE(InstanceBuffer)

    ~InstanceBuffer() = default;
};

// --------------------------- UniformBuffer ---------------------------

class UniformBuffer
{
public:
    explicit UniformBuffer() = default;

    [[nodiscard]] static auto create_static_with_size(u32 size_bytes) -> UniformBuffer;
    [[nodiscard]] static auto create_static_with_size(u32 size_bytes, u32 binding_point) -> UniformBuffer;
    [[nodiscard]] static auto create_static_with_data(const void* data, u32 data_size_bytes) -> UniformBuffer;
    [[nodiscard]] static auto create_static_with_data(const void* data, u32 data_size_bytes, u32 binding_point)
        -> UniformBuffer;
    [[nodiscard]] static auto create_static_with_data(auto&& data) -> UniformBuffer;
    [[nodiscard]] static auto create_static_with_data(auto&& data, u32 binding_point) -> UniformBuffer;
    [[nodiscard]] static auto create_static_with_data(std::ranges::contiguous_range auto&& data) -> UniformBuffer;
    [[nodiscard]] static auto create_static_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point)
        -> UniformBuffer;

    ZTH_NO_COPY(UniformBuffer)
    ZTH_DEFAULT_MOVE(UniformBuffer)

    ~UniformBuffer() = default;

    auto init_static_with_size(u32 size_bytes) -> void;
    auto init_static_with_size(u32 size_bytes, u32 binding_point) -> void;
    auto init_static_with_data(const void* data, u32 data_size_bytes) -> void;
    auto init_static_with_data(const void* data, u32 data_size_bytes, u32 binding_point) -> void;
    auto init_static_with_data(auto&& data) -> void;
    auto init_static_with_data(auto&& data, u32 binding_point) -> void;
    auto init_static_with_data(std::ranges::contiguous_range auto&& data) -> void;
    auto init_static_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point) -> void;

    auto buffer_data(const void* data, u32 data_size_bytes, u32 offset = 0) -> u32;
    auto buffer_data(auto&& data, u32 offset = 0) -> u32;
    auto buffer_data(std::ranges::contiguous_range auto&& data, u32 offset = 0) -> u32;

    auto bind() const -> void;
    auto bind(u32 binding_point) const -> void;
    static auto unbind() -> void;

    [[nodiscard]] auto native_handle() const { return _buffer.native_handle(); }
    [[nodiscard]] auto size_bytes() const { return _buffer.size_bytes(); }
    [[nodiscard]] auto is_static() const { return _buffer.is_static(); }
    [[nodiscard]] auto is_dynamic() const { return _buffer.is_dynamic(); }
    [[nodiscard]] auto is_initialized() const { return _buffer.is_initialized(); }

private:
    Buffer _buffer;
};

// --------------------------- ShaderStorageBuffer ---------------------------

class ShaderStorageBuffer
{
public:
    explicit ShaderStorageBuffer() = default;

    [[nodiscard]] static auto create_static_with_size(u32 size_bytes) -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_static_with_size(u32 size_bytes, u32 binding_point) -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_static_with_data(const void* data, u32 data_size_bytes) -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_static_with_data(const void* data, u32 data_size_bytes, u32 binding_point)
        -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_static_with_data(auto&& data) -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_static_with_data(auto&& data, u32 binding_point) -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_static_with_data(std::ranges::contiguous_range auto&& data) -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_static_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point)
        -> ShaderStorageBuffer;

    [[nodiscard]] static auto create_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_dynamic(u32 binding_point, BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_dynamic_with_size(u32 size_bytes, BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_dynamic_with_size(u32 size_bytes, u32 binding_point,
                                                       BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_dynamic_with_data(const void* data, u32 data_size_bytes,
                                                       BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_dynamic_with_data(const void* data, u32 data_size_bytes, u32 binding_point,
                                                       BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_dynamic_with_data(auto&& data, BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_dynamic_with_data(auto&& data, u32 binding_point,
                                                       BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_dynamic_with_data(std::ranges::contiguous_range auto&& data,
                                                       BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;
    [[nodiscard]] static auto create_dynamic_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point,
                                                       BufferUsage usage = BufferUsage::dynamic_draw)
        -> ShaderStorageBuffer;

    ZTH_NO_COPY(ShaderStorageBuffer)
    ZTH_DEFAULT_MOVE(ShaderStorageBuffer)

    ~ShaderStorageBuffer() = default;

    auto init_static_with_size(u32 size_bytes) -> void;
    auto init_static_with_size(u32 size_bytes, u32 binding_point) -> void;
    auto init_static_with_data(const void* data, u32 data_size_bytes) -> void;
    auto init_static_with_data(const void* data, u32 data_size_bytes, u32 binding_point) -> void;
    auto init_static_with_data(auto&& data) -> void;
    auto init_static_with_data(auto&& data, u32 binding_point) -> void;
    auto init_static_with_data(std::ranges::contiguous_range auto&& data) -> void;
    auto init_static_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point) -> void;

    auto init_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic(u32 binding_point, BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic_with_size(u32 size_bytes, BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic_with_size(u32 size_bytes, u32 binding_point, BufferUsage usage = BufferUsage::dynamic_draw)
        -> void;
    auto init_dynamic_with_data(const void* data, u32 data_size_bytes, BufferUsage usage = BufferUsage::dynamic_draw)
        -> void;
    auto init_dynamic_with_data(const void* data, u32 data_size_bytes, u32 binding_point,
                                BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic_with_data(auto&& data, BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic_with_data(auto&& data, u32 binding_point, BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic_with_data(std::ranges::contiguous_range auto&& data,
                                BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic_with_data(std::ranges::contiguous_range auto&& data, u32 binding_point,
                                BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto buffer_data(const void* data, u32 data_size_bytes, u32 offset = 0) -> u32;
    auto buffer_data(auto&& data, u32 offset = 0) -> u32;
    auto buffer_data(std::ranges::contiguous_range auto&& data, u32 offset = 0) -> u32;

    auto bind() const -> void;
    auto bind(u32 binding_point) const -> void;
    static auto unbind() -> void;

    [[nodiscard]] auto native_handle() const { return _buffer.native_handle(); }
    [[nodiscard]] auto size_bytes() const { return _buffer.size_bytes(); }
    [[nodiscard]] auto is_static() const { return _buffer.is_static(); }
    [[nodiscard]] auto is_dynamic() const { return _buffer.is_dynamic(); }
    [[nodiscard]] auto is_initialized() const { return _buffer.is_initialized(); }

private:
    Buffer _buffer;
};

[[nodiscard]] auto to_gl_enum(BufferUsage buffer_usage) -> GLenum;

} // namespace zth::gl

#include "buffer.inl"
