#pragma once

#include <glad/glad.h>

#include "Zenith/Platform/OpenGl/fwd.hpp"

namespace zth {

class VertexArray
{
public:
    explicit VertexArray();
    explicit VertexArray(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer);
    explicit VertexArray(VertexBuffer&& vertex_buffer, IndexBuffer&& index_buffer) = delete;

    VertexArray(const VertexArray& other);
    auto operator=(const VertexArray& other) -> VertexArray&;

    VertexArray(VertexArray&& other) noexcept;
    auto operator=(VertexArray&& other) noexcept -> VertexArray&;

    ~VertexArray();

    auto bind() const -> void;
    static auto unbind() -> void;

    auto bind_vertex_buffer(const VertexBuffer& vertex_buffer) -> void;
    auto bind_vertex_buffer(VertexBuffer&& vertex_buffer) = delete;
    auto bind_index_buffer(const IndexBuffer& index_buffer) -> void;
    auto bind_index_buffer(IndexBuffer&& index_buffer) = delete;

    [[nodiscard]] auto native_handle() const { return _id; }
    [[nodiscard]] auto count() const -> GLsizei;
    [[nodiscard]] auto index_type() const -> GLenum;

    [[nodiscard]] auto vertex_buffer() const -> auto& { return *_vertex_buffer; }
    [[nodiscard]] auto index_buffer() const -> auto& { return *_index_buffer; }

private:
    GLuint _id = GL_NONE;

    const VertexBuffer* _vertex_buffer = nullptr;
    const IndexBuffer* _index_buffer = nullptr;

private:
    auto create() -> void;
    auto destroy() const -> void;

    auto bind_layout(const VertexLayout& layout) const -> void;
};

} // namespace zth
