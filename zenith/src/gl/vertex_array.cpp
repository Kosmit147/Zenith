#include "zenith/gl/vertex_array.hpp"

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/buffer.hpp"

namespace zth::gl {

VertexArray::VertexArray()
{
    create();
}

VertexArray::VertexArray(const VertexArrayLayout& layout) : VertexArray()
{
    bind_layout(layout);
}

VertexArray::VertexArray(const VertexArrayLayout& layout, const VertexBuffer& vertex_buffer,
                         const IndexBuffer& index_buffer)
    : VertexArray(layout)
{
    bind_vertex_buffer(vertex_buffer);
    bind_index_buffer(index_buffer);
}

VertexArray::VertexArray(const VertexArrayLayout& layout, const VertexBuffer& vertex_buffer,
                         const IndexBuffer& index_buffer, const InstanceBuffer& instance_buffer)
    : VertexArray(layout, vertex_buffer, index_buffer)
{
    bind_instance_buffer(instance_buffer);
}

VertexArray::VertexArray(const VertexArray& other) : VertexArray(other._layout)
{
    if (other._vertex_buffer)
        bind_vertex_buffer(*other._vertex_buffer);

    if (other._index_buffer)
        bind_index_buffer(*other._index_buffer);

    if (other._instance_buffer)
        bind_instance_buffer(*other._instance_buffer);
}

auto VertexArray::operator=(const VertexArray& other) -> VertexArray&
{
    if (this == &other)
        return *this;

    destroy();

    _vertex_buffer = nullptr;
    _index_buffer = nullptr;
    _instance_buffer = nullptr;

    create();

    if (other._vertex_buffer)
        bind_vertex_buffer(*other._vertex_buffer);

    if (other._index_buffer)
        bind_index_buffer(*other._index_buffer);

    if (other._instance_buffer)
        bind_instance_buffer(*other._instance_buffer);

    bind_layout(other._layout);

    return *this;
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : _id(other._id), _vertex_buffer(other._vertex_buffer), _index_buffer(other._index_buffer),
      _instance_buffer(other._instance_buffer), _layout(std::move(other._layout))
{
    other._id = GL_NONE;
    other._vertex_buffer = nullptr;
    other._index_buffer = nullptr;
    other._instance_buffer = nullptr;
}

auto VertexArray::operator=(VertexArray&& other) noexcept -> VertexArray&
{
    destroy();

    _id = other._id;
    _vertex_buffer = other._vertex_buffer;
    _index_buffer = other._index_buffer;
    _instance_buffer = other._instance_buffer;
    _layout = std::move(other._layout);

    other._id = GL_NONE;
    other._vertex_buffer = nullptr;
    other._index_buffer = nullptr;
    other._instance_buffer = nullptr;

    return *this;
}

VertexArray::~VertexArray()
{
    destroy();
}

auto VertexArray::bind() const -> void
{
    glBindVertexArray(_id);
}

auto VertexArray::unbind() -> void
{
    glBindVertexArray(GL_NONE);
}

auto VertexArray::bind_vertex_buffer(const VertexBuffer& vertex_buffer) -> void
{
    _vertex_buffer = &vertex_buffer;
    glVertexArrayVertexBuffer(_id, vertex_buffer_binding_index, _vertex_buffer->native_handle(), 0,
                              _vertex_buffer->stride());
}

auto VertexArray::bind_index_buffer(const IndexBuffer& index_buffer) -> void
{
    _index_buffer = &index_buffer;
    glVertexArrayElementBuffer(_id, _index_buffer->native_handle());
}

auto VertexArray::bind_instance_buffer(const InstanceBuffer& instance_buffer) -> void
{
    _instance_buffer = &instance_buffer;
    glVertexArrayVertexBuffer(_id, instance_buffer_binding_index, _instance_buffer->native_handle(), 0,
                              _instance_buffer->stride());
}

auto VertexArray::bind_layout(const VertexArrayLayout& layout) -> void
{
    _layout = layout;

    bind_vertex_buffer_layout();
    bind_instance_buffer_layout();
}

auto VertexArray::unbind_vertex_buffer() -> void
{
    glVertexArrayVertexBuffer(_id, vertex_buffer_binding_index, GL_NONE, 0, 0);
    _vertex_buffer = nullptr;
}

auto VertexArray::unbind_index_buffer() -> void
{
    glVertexArrayElementBuffer(_id, GL_NONE);
    _index_buffer = nullptr;
}

auto VertexArray::unbind_instance_buffer() -> void
{
    glVertexArrayVertexBuffer(_id, instance_buffer_binding_index, GL_NONE, 0, 0);
    _instance_buffer = nullptr;
}

auto VertexArray::unbind_all_buffers() -> void
{
    unbind_vertex_buffer();
    unbind_index_buffer();
    unbind_instance_buffer();
}

auto VertexArray::count() const -> GLsizei
{
    if (!_index_buffer)
        return 0;

    return _index_buffer->size();
}

auto VertexArray::index_type() const -> GLenum
{
    if (!_index_buffer)
        return GL_NONE;

    return _index_buffer->index_type();
}

auto VertexArray::create() -> void
{
    glCreateVertexArrays(1, &_id);
}

auto VertexArray::destroy() const -> void
{
    glDeleteVertexArrays(1, &_id);
}

auto VertexArray::bind_vertex_buffer_layout() const -> void
{
    GLuint index = 0;
    GLuint offset = 0;

    for (auto& elem : _layout.vertex_buffer_layout)
    {
        auto [count, type, size, slots_occupied] = get_vertex_layout_element_info(elem);

        for (usize i = 0; i < slots_occupied; i++)
        {
            glEnableVertexArrayAttrib(_id, index);
            glVertexArrayAttribFormat(_id, index, count, type, GL_FALSE, offset);
            glVertexArrayAttribBinding(_id, index, vertex_buffer_binding_index);

            index++;
            offset += size;
        }
    }
}

auto VertexArray::bind_instance_buffer_layout() const -> void
{
    GLuint index = static_cast<GLuint>(_layout.vertex_buffer_layout.size());
    GLuint offset = 0;

    for (auto& elem : _layout.instance_buffer_layout)
    {
        auto [count, type, size, slots_occupied] = get_vertex_layout_element_info(elem);

        for (usize i = 0; i < slots_occupied; i++)
        {
            glEnableVertexArrayAttrib(_id, index);
            glVertexArrayAttribFormat(_id, index, count, type, GL_FALSE, offset);
            glVertexArrayAttribBinding(_id, index, instance_buffer_binding_index);
            glVertexArrayBindingDivisor(_id, instance_buffer_binding_index, 1);

            index++;
            offset += size;
        }
    }
}

} // namespace zth::gl
