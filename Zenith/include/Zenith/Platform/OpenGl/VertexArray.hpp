#pragma once

#include <glad/glad.h>

#include "Zenith/Utility/Utility.hpp"

namespace zth {

class VertexBuffer;
class IndexBuffer;
class VertexLayout;

class VertexArray
{
public:
    explicit VertexArray() { create(); }
    ZTH_NO_COPY_NO_MOVE(VertexArray)
    ~VertexArray() { destroy(); }

    auto bind() const -> void { glBindVertexArray(_id); }
    auto unbind() const -> void { glBindVertexArray(0); }

    auto bind_vertex_buffer(const VertexBuffer& vertex_buffer) -> void;
    auto bind_vertex_buffer(VertexBuffer&& vertex_buffer) = delete;
    auto bind_index_buffer(const IndexBuffer& index_buffer) -> void;
    auto bind_index_buffer(IndexBuffer&& index_buffer) = delete;

    auto bind_layout(const VertexLayout& layout) const -> void;

    [[nodiscard]] auto native_handle() const { return _id; }
    [[nodiscard]] auto count() const -> GLsizei;
    [[nodiscard]] auto index_type() const -> GLenum;

    [[nodiscard]] auto vertex_buffer() const { return _vertex_buffer; }
    [[nodiscard]] auto index_buffer() const { return _index_buffer; }

private:
    GLuint _id = GL_NONE;

    const VertexBuffer* _vertex_buffer = nullptr;
    const IndexBuffer* _index_buffer = nullptr;

private:
    auto create() -> void { glCreateVertexArrays(1, &_id); }
    auto destroy() const -> void { glDeleteVertexArrays(1, &_id); }
};

} // namespace zth
