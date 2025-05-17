#include "zenith/gl/vertex_array.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/gl/buffer.hpp"
#include "zenith/gl/util.hpp"

namespace zth::gl {

VertexArray::VertexArray()
{
    create();
}

VertexArray::VertexArray(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer) : VertexArray()
{
    bind_vertex_buffer(vertex_buffer);
    bind_index_buffer(index_buffer);
    rebind_layout();
}

VertexArray::VertexArray(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer, u32 count_limit)
    : VertexArray(vertex_buffer, index_buffer)
{
    _count_limit = count_limit;
}

VertexArray::VertexArray(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer,
                         const InstanceBuffer& instance_buffer)
    : VertexArray()
{
    bind_vertex_buffer(vertex_buffer);
    bind_index_buffer(index_buffer);
    bind_instance_buffer(instance_buffer);
    rebind_layout();
}

VertexArray::VertexArray(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer,
                         const InstanceBuffer& instance_buffer, u32 count_limit)
    : VertexArray(vertex_buffer, index_buffer, instance_buffer)
{
    _count_limit = count_limit;
}

VertexArray::VertexArray(const VertexArray& other) : VertexArray()
{
    if (other._vertex_buffer)
        bind_vertex_buffer(*other._vertex_buffer);

    if (other._index_buffer)
        bind_index_buffer(*other._index_buffer);

    if (other._instance_buffer)
        bind_instance_buffer(*other._instance_buffer);

    rebind_layout();
    _count_limit = other._count_limit;
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

    rebind_layout();
    _count_limit = other._count_limit;

    return *this;
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : _id(std::exchange(other._id, GL_NONE)), _count_limit(std::exchange(other._count_limit, nil)),
      _vertex_buffer(std::exchange(other._vertex_buffer, nullptr)),
      _index_buffer(std::exchange(other._index_buffer, nullptr)),
      _instance_buffer(std::exchange(other._instance_buffer, nullptr))
{}

auto VertexArray::operator=(VertexArray&& other) noexcept -> VertexArray&
{
    destroy();

    _id = std::exchange(other._id, GL_NONE);
    _count_limit = std::exchange(other._count_limit, nil);
    _vertex_buffer = std::exchange(other._vertex_buffer, nullptr);
    _index_buffer = std::exchange(other._index_buffer, nullptr);
    _instance_buffer = std::exchange(other._instance_buffer, nullptr);

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
                              static_cast<GLsizei>(_vertex_buffer->stride()));
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
                              static_cast<GLsizei>(_instance_buffer->stride()));
}

auto VertexArray::rebind_layout() -> void
{
    if (_vertex_buffer)
        bind_vertex_buffer_layout();

    if (_instance_buffer)
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

auto VertexArray::set_count_limit(u32 limit) -> void
{
    _count_limit = limit;
}

auto VertexArray::set_count_limit(Nil) -> void
{
    _count_limit = nil;
}

auto VertexArray::count() const -> u32
{
    if (!_index_buffer)
        return 0;

    if (_count_limit)
        return std::min(*_count_limit, _index_buffer->count());

    return _index_buffer->count();
}

auto VertexArray::indexing_data_type() const -> DataType
{
    if (!_index_buffer)
        return DataType::UnsignedByte;

    return _index_buffer->indexing_data_type();
}

auto VertexArray::layout() const -> VertexArrayLayout
{
    VertexArrayLayout layout{};

    if (_vertex_buffer)
        layout.vertex_buffer_layout = _vertex_buffer->layout();

    if (_instance_buffer)
        layout.instance_buffer_layout = _instance_buffer->layout();

    return layout;
}

auto VertexArray::create() noexcept -> void
{
    glCreateVertexArrays(1, &_id);
}

auto VertexArray::destroy() const noexcept -> void
{
    glDeleteVertexArrays(1, &_id);
}

auto VertexArray::bind_vertex_buffer_layout() const -> void
{
    ZTH_ASSERT(_vertex_buffer != nullptr);

    auto& layout = _vertex_buffer->layout();

    GLuint index = 0;
    GLuint offset = 0;

    for (auto& elem : layout)
    {
        auto [count, type, size, slots_occupied] = get_vertex_layout_element_info(elem);

        for (GLuint i = 0; i < slots_occupied; i++)
        {
            glEnableVertexArrayAttrib(_id, index);
            glVertexArrayAttribFormat(_id, index, static_cast<GLint>(count), to_gl_enum(type), GL_FALSE, offset);
            glVertexArrayAttribBinding(_id, index, vertex_buffer_binding_index);

            index++;
            offset += size;
        }
    }
}

auto VertexArray::bind_instance_buffer_layout() const -> void
{
    ZTH_ASSERT(_instance_buffer != nullptr);

    auto& layout = _instance_buffer->layout();

    GLuint index = 0;
    GLuint offset = 0;

    if (_vertex_buffer)
        index += static_cast<GLuint>(_vertex_buffer->layout().count());

    for (auto& elem : layout)
    {
        auto [count, type, size, slots_occupied] = get_vertex_layout_element_info(elem);

        for (GLuint i = 0; i < slots_occupied; i++)
        {
            glEnableVertexArrayAttrib(_id, index);
            glVertexArrayAttribFormat(_id, index, static_cast<GLint>(count), to_gl_enum(type), GL_FALSE, offset);
            glVertexArrayAttribBinding(_id, index, instance_buffer_binding_index);
            glVertexArrayBindingDivisor(_id, instance_buffer_binding_index, 1);

            index++;
            offset += size;
        }
    }
}

} // namespace zth::gl
