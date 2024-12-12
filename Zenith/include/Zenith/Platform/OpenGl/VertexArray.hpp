#pragma once

#include <glad/glad.h>

#include "Zenith/Platform/OpenGl/fwd.hpp"

namespace zth {

class VertexArray
{
public:
    static constexpr GLuint vertex_buffer_binding_index = 0;
    static constexpr GLuint instance_buffer_binding_index = 1;

public:
    explicit VertexArray();
    explicit VertexArray(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer);
    explicit VertexArray(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer,
                         const InstanceBuffer& instance_buffer);

    explicit VertexArray(VertexBuffer&&, IndexBuffer&&) = delete;
    explicit VertexArray(VertexBuffer&&, IndexBuffer&&, InstanceBuffer&&) = delete;

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

    auto unbind_vertex_buffer() -> void;
    auto unbind_index_buffer() -> void;
    auto unbind_instance_buffer() -> void;
    auto unbind_all_buffers() -> void;

    [[nodiscard]] auto native_handle() const { return _id; }
    [[nodiscard]] auto count() const -> GLsizei;
    [[nodiscard]] auto index_type() const -> GLenum;

    [[nodiscard]] auto vertex_buffer() const { return _vertex_buffer; }
    [[nodiscard]] auto index_buffer() const { return _index_buffer; }
    [[nodiscard]] auto instance_buffer() const { return _instance_buffer; }

private:
    GLuint _id = GL_NONE;

    const VertexBuffer* _vertex_buffer = nullptr;
    const IndexBuffer* _index_buffer = nullptr;
    const InstanceBuffer* _instance_buffer = nullptr;

private:
    auto create() -> void;
    auto destroy() const -> void;

    auto bind_layouts() const -> void;
    auto bind_vertex_buffer_layout(const VertexLayout& layout) const -> void;
    auto bind_instance_buffer_layout(const VertexLayout& layout) const -> void;
};

} // namespace zth
