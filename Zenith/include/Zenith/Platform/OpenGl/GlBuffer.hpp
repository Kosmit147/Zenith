#pragma once

#include <glad/glad.h>

#include <optional>
#include <ranges>

#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Platform/OpenGl/GlBufferUsage.hpp"
#include "Zenith/Utility/Macros.hpp"

namespace zth {

enum class GlBufferState : u8
{
    Uninitialized,
    InitializedStatic,  // buffer cannot be reinitialized, size cannot be changed
    InitializedDynamic, // buffer can be reinitialized, size can be changed
};

class GlBuffer
{
public:
    explicit GlBuffer();

    [[nodiscard]] static auto create_static(usize size) -> GlBuffer;
    [[nodiscard]] static auto create_static(std::ranges::contiguous_range auto&& data) -> GlBuffer;

    [[nodiscard]] static auto create_dynamic(GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> GlBuffer;
    [[nodiscard]] static auto create_dynamic(usize size, GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> GlBuffer;
    [[nodiscard]] static auto create_dynamic(std::ranges::contiguous_range auto&& data,
                                             GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> GlBuffer;

    ZTH_NO_COPY(GlBuffer)

    GlBuffer(GlBuffer&& other) noexcept;
    auto operator=(GlBuffer&& other) noexcept -> GlBuffer&;

    ~GlBuffer();

    auto init_static(usize size) -> void;
    auto init_static(std::ranges::contiguous_range auto&& data) -> void;

    auto init_dynamic(GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> void;
    auto init_dynamic(usize size, GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> void;
    auto init_dynamic(std::ranges::contiguous_range auto&& data,
                      GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> void;

    auto buffer_data(std::ranges::contiguous_range auto&& data, usize offset = 0) -> void;
    auto buffer_data(auto&& object, usize offset = 0) -> void;
    auto buffer_data(const void* data, usize offset, usize data_size_bytes) -> void;

    [[nodiscard]] auto native_handle() const { return _id; }
    [[nodiscard]] auto size_bytes() const { return _size_bytes; }
    [[nodiscard]] auto is_static() const { return _state == GlBufferState::InitializedStatic; }
    [[nodiscard]] auto is_dynamic() const { return _state == GlBufferState::InitializedDynamic; }
    [[nodiscard]] auto is_initialized() const { return _state != GlBufferState::Uninitialized; }

private:
    GLuint _id = GL_NONE;
    GLsizei _size_bytes = 0;
    GlBufferState _state = GlBufferState::Uninitialized;
    std::optional<GlBufferUsage> _usage = std::nullopt;

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

    [[nodiscard]] static auto create_dynamic(GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> VertexBuffer;
    [[nodiscard]] static auto create_dynamic(usize size,
                                             GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> VertexBuffer;
    [[nodiscard]] static auto create_dynamic(std::ranges::contiguous_range auto&& data,
                                             GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> VertexBuffer;

    ZTH_NO_COPY(VertexBuffer)

    VertexBuffer(VertexBuffer&& other) noexcept;
    auto operator=(VertexBuffer&& other) noexcept -> VertexBuffer&;

    ~VertexBuffer() = default;

    auto init_static(usize size) -> void;
    auto init_static(std::ranges::contiguous_range auto&& data) -> void;

    auto init_dynamic(GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> void;
    auto init_dynamic(usize size, GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> void;
    auto init_dynamic(std::ranges::contiguous_range auto&& data,
                      GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> void;

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
    GlBuffer _buffer;
    GLsizei _stride = 0;
};

class IndexBuffer
{
public:
    explicit IndexBuffer() = default;

    [[nodiscard]] static auto create_static(usize size) -> IndexBuffer;
    [[nodiscard]] static auto create_static(std::ranges::contiguous_range auto&& data) -> IndexBuffer;

    [[nodiscard]] static auto create_dynamic(GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> IndexBuffer;
    [[nodiscard]] static auto create_dynamic(usize size,
                                             GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> IndexBuffer;
    [[nodiscard]] static auto create_dynamic(std::ranges::contiguous_range auto&& data,
                                             GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> IndexBuffer;

    ZTH_NO_COPY(IndexBuffer)

    IndexBuffer(IndexBuffer&& other) noexcept;
    auto operator=(IndexBuffer&& other) noexcept -> IndexBuffer&;

    ~IndexBuffer() = default;

    auto init_static(usize size) -> void;
    auto init_static(std::ranges::contiguous_range auto&& data) -> void;

    auto init_dynamic(GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> void;
    auto init_dynamic(usize size, GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> void;
    auto init_dynamic(std::ranges::contiguous_range auto&& data,
                      GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> void;

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
    GlBuffer _buffer;
    GLenum _index_type = GL_NONE;
    GLsizei _size = 0;
};

class InstanceBuffer : public VertexBuffer
{
public:
    explicit InstanceBuffer() = default;

    [[nodiscard]] static auto create_static(usize size) -> InstanceBuffer;
    [[nodiscard]] static auto create_static(std::ranges::contiguous_range auto&& data) -> InstanceBuffer;

    [[nodiscard]] static auto create_dynamic(GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> InstanceBuffer;
    [[nodiscard]] static auto create_dynamic(usize size,
                                             GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> InstanceBuffer;
    [[nodiscard]] static auto create_dynamic(std::ranges::contiguous_range auto&& data,
                                             GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> InstanceBuffer;

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
    GlBuffer _buffer;
};

} // namespace zth

#include "GlBuffer.inl"
