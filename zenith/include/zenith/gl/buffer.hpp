#pragma once

#include <glad/glad.h>

#include <optional>
#include <ranges>

#include "zenith/core/typedefs.hpp"
#include "zenith/util/macros.hpp"

namespace zth::gl {

enum class BufferAccessFrequency : u8
{
    // @volatile: These numbers are chosen for converting BufferUsage to an OpenGL value

    Stream = 0,  // The data store contents will be modified once and used at most a few times.
    Static = 4,  // The data store contents will be modified once and used many times.
    Dynamic = 8, // The data store contents will be modified repeatedly and used many times.
};

enum class BufferAccessType : u8
{
    // @volatile: These numbers are chosen for converting BufferUsage to an OpenGL value

    Draw = 0, // The data store contents are modified by the application, and used as the source for GL drawing and
              // image specification commands.
    Read = 1, // The data store contents are modified by reading data from the GL, and used to return that data when
              // queried by the application.
    Copy = 2, // The data store contents are modified by reading data from the GL, and used as the source for GL drawing
              // and image specification commands.
};

struct BufferUsage
{
    BufferAccessFrequency access_frequency;
    BufferAccessType access_type;

    static const BufferUsage stream_draw;
    static const BufferUsage static_draw;
    static const BufferUsage dynamic_draw;

    static const BufferUsage stream_read;
    static const BufferUsage static_read;
    static const BufferUsage dynamic_read;

    static const BufferUsage stream_copy;
    static const BufferUsage static_copy;
    static const BufferUsage dynamic_copy;
};

enum class BufferState : u8
{
    Uninitialized,
    InitializedStatic,  // buffer cannot be reinitialized, size cannot be changed
    InitializedDynamic, // buffer can be reinitialized, size can be changed
};

class Buffer
{
public:
    explicit Buffer();

    [[nodiscard]] static auto create_static(usize size) -> Buffer;
    [[nodiscard]] static auto create_static(std::ranges::contiguous_range auto&& data) -> Buffer;

    [[nodiscard]] static auto create_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> Buffer;
    [[nodiscard]] static auto create_dynamic(usize size, BufferUsage usage = BufferUsage::dynamic_draw) -> Buffer;
    [[nodiscard]] static auto create_dynamic(std::ranges::contiguous_range auto&& data,
                                             BufferUsage usage = BufferUsage::dynamic_draw) -> Buffer;

    ZTH_NO_COPY(Buffer)

    Buffer(Buffer&& other) noexcept;
    auto operator=(Buffer&& other) noexcept -> Buffer&;

    ~Buffer();

    auto init_static(usize size) -> void;
    auto init_static(std::ranges::contiguous_range auto&& data) -> void;

    auto init_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic(usize size, BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic(std::ranges::contiguous_range auto&& data, BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto buffer_data(std::ranges::contiguous_range auto&& data, usize offset = 0) -> void;
    auto buffer_data(auto&& object, usize offset = 0) -> void;
    auto buffer_data(const void* data, usize offset, usize data_size_bytes) -> void;

    [[nodiscard]] auto native_handle() const { return _id; }
    [[nodiscard]] auto size_bytes() const { return _size_bytes; }
    [[nodiscard]] auto is_static() const { return _state == BufferState::InitializedStatic; }
    [[nodiscard]] auto is_dynamic() const { return _state == BufferState::InitializedDynamic; }
    [[nodiscard]] auto is_initialized() const { return _state != BufferState::Uninitialized; }

private:
    GLuint _id = GL_NONE;
    GLsizei _size_bytes = 0;
    BufferState _state = BufferState::Uninitialized;
    std::optional<BufferUsage> _usage = std::nullopt;

private:
    auto create() -> void;
    auto destroy() -> void;

    auto resize_to_at_least(GLsizei min_size_bytes) -> void;
};

class VertexBuffer
{
public:
    explicit VertexBuffer() = default;

    [[nodiscard]] static auto create_static(usize size) -> VertexBuffer;
    [[nodiscard]] static auto create_static(std::ranges::contiguous_range auto&& data) -> VertexBuffer;

    [[nodiscard]] static auto create_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> VertexBuffer;
    [[nodiscard]] static auto create_dynamic(usize size, BufferUsage usage = BufferUsage::dynamic_draw) -> VertexBuffer;
    [[nodiscard]] static auto create_dynamic(std::ranges::contiguous_range auto&& data,
                                             BufferUsage usage = BufferUsage::dynamic_draw) -> VertexBuffer;

    ZTH_NO_COPY(VertexBuffer)

    VertexBuffer(VertexBuffer&& other) noexcept;
    auto operator=(VertexBuffer&& other) noexcept -> VertexBuffer&;

    ~VertexBuffer() = default;

    auto init_static(usize size) -> void;
    auto init_static(std::ranges::contiguous_range auto&& data) -> void;

    auto init_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic(usize size, BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic(std::ranges::contiguous_range auto&& data, BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto buffer_data(std::ranges::contiguous_range auto&& data, usize offset = 0) -> void;

    auto bind() const -> void;
    static auto unbind() -> void;

    auto set_stride(GLsizei stride) -> void;

    [[nodiscard]] auto native_handle() const { return _buffer.native_handle(); }
    [[nodiscard]] auto size_bytes() const { return _buffer.size_bytes(); }
    [[nodiscard]] auto is_static() const { return _buffer.is_static(); }
    [[nodiscard]] auto is_dynamic() const { return _buffer.is_dynamic(); }
    [[nodiscard]] auto is_initialized() const { return _buffer.is_initialized(); }
    [[nodiscard]] auto stride() const { return _stride; }

private:
    Buffer _buffer;
    GLsizei _stride = 0;
};

class IndexBuffer
{
public:
    explicit IndexBuffer() = default;

    [[nodiscard]] static auto create_static(usize size) -> IndexBuffer;
    [[nodiscard]] static auto create_static(std::ranges::contiguous_range auto&& data) -> IndexBuffer;

    [[nodiscard]] static auto create_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> IndexBuffer;
    [[nodiscard]] static auto create_dynamic(usize size, BufferUsage usage = BufferUsage::dynamic_draw) -> IndexBuffer;
    [[nodiscard]] static auto create_dynamic(std::ranges::contiguous_range auto&& data,
                                             BufferUsage usage = BufferUsage::dynamic_draw) -> IndexBuffer;

    ZTH_NO_COPY(IndexBuffer)

    IndexBuffer(IndexBuffer&& other) noexcept;
    auto operator=(IndexBuffer&& other) noexcept -> IndexBuffer&;

    ~IndexBuffer() = default;

    auto init_static(usize size) -> void;
    auto init_static(std::ranges::contiguous_range auto&& data) -> void;

    auto init_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic(usize size, BufferUsage usage = BufferUsage::dynamic_draw) -> void;
    auto init_dynamic(std::ranges::contiguous_range auto&& data, BufferUsage usage = BufferUsage::dynamic_draw) -> void;

    auto buffer_data(std::ranges::contiguous_range auto&& data, usize offset = 0) -> void;

    auto bind() const -> void;
    static auto unbind() -> void;

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
    GLsizei _size = 0;
};

class InstanceBuffer : public VertexBuffer
{
public:
    explicit InstanceBuffer() = default;

    [[nodiscard]] static auto create_static(usize size) -> InstanceBuffer;
    [[nodiscard]] static auto create_static(std::ranges::contiguous_range auto&& data) -> InstanceBuffer;

    [[nodiscard]] static auto create_dynamic(BufferUsage usage = BufferUsage::dynamic_draw) -> InstanceBuffer;
    [[nodiscard]] static auto create_dynamic(usize size,
                                             BufferUsage usage = BufferUsage::dynamic_draw) -> InstanceBuffer;
    [[nodiscard]] static auto create_dynamic(std::ranges::contiguous_range auto&& data,
                                             BufferUsage usage = BufferUsage::dynamic_draw) -> InstanceBuffer;

    ZTH_NO_COPY(InstanceBuffer)
    ZTH_DEFAULT_MOVE(InstanceBuffer)

    ~InstanceBuffer() = default;
};

class UniformBuffer
{
public:
    explicit UniformBuffer() = default;

    [[nodiscard]] static auto create_static(usize size) -> UniformBuffer;
    [[nodiscard]] static auto create_static(usize size, u32 binding_index) -> UniformBuffer;
    [[nodiscard]] static auto create_static(std::ranges::contiguous_range auto&& data) -> UniformBuffer;
    [[nodiscard]] static auto create_static(std::ranges::contiguous_range auto&& data,
                                            u32 binding_index) -> UniformBuffer;

    ZTH_NO_COPY(UniformBuffer)
    ZTH_DEFAULT_MOVE(UniformBuffer)

    ~UniformBuffer() = default;

    auto init_static(usize size) -> void;
    auto init_static(usize size, u32 binding_index) -> void;
    auto init_static(std::ranges::contiguous_range auto&& data) -> void;
    auto init_static(std::ranges::contiguous_range auto&& data, u32 binding_index) -> void;

    auto buffer_data(std::ranges::contiguous_range auto&& data, usize offset = 0) -> void;
    auto buffer_data(auto&& object, usize offset = 0) -> void;
    auto buffer_data(const void* data, usize offset, usize size_bytes) -> void;

    auto bind() const -> void;
    static auto unbind() -> void;

    auto set_binding_index(u32 index) const -> void;

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
