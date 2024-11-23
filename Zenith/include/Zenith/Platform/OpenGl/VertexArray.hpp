#pragma once

#include <glad/glad.h>

#include "Zenith/Platform/OpenGl/GlBuffer.hpp"
#include "Zenith/Platform/OpenGl/VertexLayout.hpp"
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

    auto bind_layout(const VertexLayout& layout) const -> void
    {
        GLuint index = 0;
        GLuint offset = 0;

        for (auto& elem : layout)
        {
            auto [count, type, size] = get_vertex_layout_element_info(elem);

            glEnableVertexArrayAttrib(_id, index);
            glVertexArrayAttribFormat(_id, index, count, type, GL_FALSE, offset);
            glVertexArrayAttribBinding(_id, index, 0);

            index++;
            offset += size;
        }
    }

    [[nodiscard]] auto native_handle() const { return _id; }

private:
    GLuint _id = GL_NONE;

private:
    auto create() -> void { glCreateVertexArrays(1, &_id); }
    auto destroy() const -> void { glDeleteVertexArrays(1, &_id); }
};

} // namespace zth
