#pragma once

#include <glad/glad.h>

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/fwd.hpp"
#include "zenith/gl/vertex_layout.hpp"
#include "zenith/util/optional.hpp"

namespace zth::gl {

struct VertexArrayLayout
{
    VertexLayout vertex_buffer_layout{};
    VertexLayout instance_buffer_layout{};
};

// We're using separate attribute format from OpenGL 4.3 and newer, which means that vertex layouts are bound to vertex
// arrays instead of vertex buffers. Therefore, you can use the same vertex array with different vertex buffers without
// having to specify the layout again as long as the layouts of the vertex buffers are the same.
//
// The instance buffer's layout starts at index one past the last index of the vertex buffer's layout.
// rebind_layout() should be called after binding a vertex buffer or an instance buffer.
//
// If the count of vertices to draw is not set, the count() method returns the number of indices in the index buffer.
class VertexArray
{
public:
    using VertexArrayId = GLuint;

    static constexpr u32 vertex_buffer_binding_index = 0;
    static constexpr u32 instance_buffer_binding_index = 1;

public:
    explicit VertexArray();

    explicit VertexArray(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer);
    explicit VertexArray(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer, u32 count);

    explicit VertexArray(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer,
                         const InstanceBuffer& instance_buffer);
    explicit VertexArray(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer,
                         const InstanceBuffer& instance_buffer, u32 count);

    explicit VertexArray(VertexBuffer&&, IndexBuffer&&) = delete;
    explicit VertexArray(VertexBuffer&&, IndexBuffer&&, u32) = delete;
    explicit VertexArray(VertexBuffer&&, IndexBuffer&&, InstanceBuffer&&) = delete;
    explicit VertexArray(VertexBuffer&&, IndexBuffer&&, InstanceBuffer&&, u32) = delete;

    VertexArray(const VertexArray& other);
    auto operator=(const VertexArray& other) -> VertexArray&;

    VertexArray(VertexArray&& other) noexcept;
    auto operator=(VertexArray&& other) noexcept -> VertexArray&;

    ~VertexArray();

    auto bind() const -> void;
    static auto unbind() -> void;

    auto bind_vertex_buffer(const VertexBuffer& vertex_buffer) -> void;
    auto bind_index_buffer(const IndexBuffer& index_buffer) -> void;
    auto bind_instance_buffer(const InstanceBuffer& instance_buffer) -> void;

    auto bind_vertex_buffer(VertexBuffer&&) = delete;
    auto bind_index_buffer(IndexBuffer&&) = delete;
    auto bind_instance_buffer(InstanceBuffer&&) = delete;

    auto rebind_layout() -> void;

    auto unbind_vertex_buffer() -> void;
    auto unbind_index_buffer() -> void;
    auto unbind_instance_buffer() -> void;
    auto unbind_all_buffers() -> void;

    auto set_count(u32 count) -> void;
    auto set_count(Nil) -> void;

    [[nodiscard]] auto native_handle() const { return _id; }
    [[nodiscard]] auto count() const -> u32;
    [[nodiscard]] auto indexing_data_type() const -> DataType;

    // @todo: These should return optionals.
    [[nodiscard]] auto vertex_buffer() const { return _vertex_buffer; }
    [[nodiscard]] auto index_buffer() const { return _index_buffer; }
    [[nodiscard]] auto instance_buffer() const { return _instance_buffer; }

    [[nodiscard]] auto layout() const -> VertexArrayLayout;

private:
    VertexArrayId _id = GL_NONE;
    Optional<u32> _count = nil;

    const VertexBuffer* _vertex_buffer = nullptr;
    const IndexBuffer* _index_buffer = nullptr;
    const InstanceBuffer* _instance_buffer = nullptr;

private:
    auto create() noexcept -> void;
    auto destroy() const noexcept -> void;

    auto bind_vertex_buffer_layout() const -> void;
    auto bind_instance_buffer_layout() const -> void;
};

} // namespace zth::gl
