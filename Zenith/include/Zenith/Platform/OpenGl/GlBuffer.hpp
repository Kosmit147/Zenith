#pragma once

#include <glad/glad.h>

#include <optional>
#include <ranges>

#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Platform/OpenGl/GlBufferUsage.hpp"
#include "Zenith/Platform/OpenGl/VertexBufferLayout.hpp"
#include "Zenith/Utility/Utility.hpp"

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
    ZTH_NO_COPY(GlBuffer)
    ~GlBuffer();

    [[nodiscard]] auto native_handle() const { return _id; }
    [[nodiscard]] auto size_bytes() const { return _size_bytes; }
    [[nodiscard]] auto is_static() const { return _state == GlBufferState::InitializedStatic; }
    [[nodiscard]] auto is_dynamic() const { return _state == GlBufferState::InitializedDynamic; }
    [[nodiscard]] auto is_initialized() const { return _state != GlBufferState::Uninitialized; }

protected:
    // these protected functions should be implemented in derived classes

    static auto create_static(usize size) -> GlBuffer;
    static auto create_static(std::ranges::contiguous_range auto&& data) -> GlBuffer;

    static auto create_dynamic(GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> GlBuffer;
    static auto create_dynamic(usize size, GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> GlBuffer;
    static auto create_dynamic(std::ranges::contiguous_range auto&& data,
                               GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> GlBuffer;

    GlBuffer(GlBuffer&& other) noexcept;
    auto operator=(GlBuffer&& other) noexcept -> GlBuffer&;

    auto init_static(usize size) -> void;
    auto init_static(std::ranges::contiguous_range auto&& data) -> void;

    auto init_dynamic(GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> void;
    auto init_dynamic(usize size, GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> void;
    auto init_dynamic(std::ranges::contiguous_range auto&& data,
                      GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> void;

    auto buffer_data(std::ranges::contiguous_range auto&& data, usize offset = 0) -> void;
    auto buffer_data(auto&& object, usize offset = 0) -> void;
    auto buffer_data(const void* data, usize offset, usize data_size_bytes) -> void;

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

class VertexBuffer : public GlBuffer
{
public:
    explicit VertexBuffer() = default;

    static auto create_static(usize size) -> VertexBuffer;
    static auto create_static(std::ranges::contiguous_range auto&& data) -> VertexBuffer;

    static auto create_dynamic(GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> VertexBuffer;
    static auto create_dynamic(usize size, GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> VertexBuffer;
    static auto create_dynamic(std::ranges::contiguous_range auto&& data,
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

    auto set_layout(const VertexBufferLayout& layout) -> void;
    auto set_layout(VertexBufferLayout&& layout) -> void;
    auto set_stride(GLsizei stride) -> void;

    [[nodiscard]] auto layout() const -> auto& { return _layout; }
    [[nodiscard]] auto stride() const { return _stride; }

private:
    VertexBufferLayout _layout = {};
    GLsizei _stride = 0;
};

class IndexBuffer : public GlBuffer
{
public:
    explicit IndexBuffer() = default;

    static auto create_static(usize size) -> IndexBuffer;
    static auto create_static(std::ranges::contiguous_range auto&& data) -> IndexBuffer;

    static auto create_dynamic(GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> IndexBuffer;
    static auto create_dynamic(usize size, GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> IndexBuffer;
    static auto create_dynamic(std::ranges::contiguous_range auto&& data,
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

    [[nodiscard]] auto index_type() const { return _index_type; }
    [[nodiscard]] auto size() const { return _size; }

private:
    GLenum _index_type = GL_NONE;
    GLsizei _size = 0;
};

class UniformBuffer : public GlBuffer
{
public:
    explicit UniformBuffer() = default;

    static auto create_static(usize size) -> UniformBuffer;
    static auto create_static(std::ranges::contiguous_range auto&& data) -> UniformBuffer;

    ZTH_NO_COPY(UniformBuffer)
    ZTH_DEFAULT_MOVE(UniformBuffer)

    ~UniformBuffer() = default;

    auto init_static(usize size) -> void;
    auto init_static(std::ranges::contiguous_range auto&& data) -> void;

    auto buffer_data(std::ranges::contiguous_range auto&& data, usize offset = 0) -> void;
    auto buffer_data(auto&& object, usize offset = 0) -> void;
    auto buffer_data(const void* data, usize offset, usize size_bytes) -> void;

    auto bind() const -> void;
    static auto unbind() -> void;

    auto set_binding_index(GLuint index) const -> void;
};

class InstanceBuffer : public VertexBuffer
{
public:
    explicit InstanceBuffer() = default;

    static auto create_static(usize size) -> InstanceBuffer;
    static auto create_static(std::ranges::contiguous_range auto&& data) -> InstanceBuffer;

    static auto create_dynamic(GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> InstanceBuffer;
    static auto create_dynamic(usize size, GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> InstanceBuffer;
    static auto create_dynamic(std::ranges::contiguous_range auto&& data,
                               GlBufferUsage usage = GlBufferUsage::dynamic_draw) -> InstanceBuffer;

    ZTH_NO_COPY(InstanceBuffer)
    ZTH_DEFAULT_MOVE(InstanceBuffer)

    ~InstanceBuffer() = default;
};

} // namespace zth

#include "GlBuffer.inl"
