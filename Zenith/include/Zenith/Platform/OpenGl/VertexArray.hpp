#pragma once

#include <glad/glad.h>

#include "Zenith/Platform/OpenGl/GlBuffer.hpp"
#include "Zenith/Utility/Utility.hpp"

namespace zth {

class VertexArray
{
public:
    explicit VertexArray() { create(); }
    ZTH_NO_COPY_NO_MOVE(VertexArray)
    ~VertexArray() { destroy(); }

    auto bind() const -> void { glBindVertexArray(_id); }
    auto unbind() const -> void { glBindVertexArray(0); }

    template<typename VertexType> auto bind_vertex_buffer(const VertexBuffer<VertexType>& vertex_buffer)
    {
        glVertexArrayVertexBuffer(_id, 0, vertex_buffer.native_handle(), 0, sizeof(VertexType));
    }

    template<typename VertexType> auto bind_vertex_buffer(VertexBuffer<VertexType>&& vertex_buffer) = delete;

    template<typename IndexType> auto bind_index_buffer(const IndexBuffer<IndexType>& index_buffer)
    {
        glVertexArrayElementBuffer(_id, index_buffer.native_handle());
    }

    template<typename IndexType> auto bind_index_buffer(IndexBuffer<IndexType>&& index_buffer) = delete;

    [[nodiscard]] auto native_handle() const { return _id; }

private:
    GLuint _id = 0; // 0 is an invalid vertex array id

private:
    auto create() -> void { glCreateVertexArrays(1, &_id); }
    auto destroy() const -> void { glDeleteVertexArrays(1, &_id); }
};

} // namespace zth
