#include "zenith/renderer/primitives.hpp"

#include "zenith/core/assert.hpp"

namespace zth {

auto create_static_index_buffer_for_quads(usize quad_count) -> gl::IndexBuffer
{
    auto indexing_data_type = gl::smallest_indexing_data_type(max_value_of_quads_index_buffer_index(quad_count));

    switch (indexing_data_type)
    {
        using enum gl::DataType;
    case UnsignedByte:
    {
        static_assert(gl::size_of_data_type(UnsignedByte) == sizeof(GLubyte));
        auto indices = generate_indices_for_quads<GLubyte>(quad_count);
        return gl::IndexBuffer::create_static_with_data(indices);
    }
    case UnsignedShort:
    {
        static_assert(gl::size_of_data_type(UnsignedShort) == sizeof(GLushort));
        auto indices = generate_indices_for_quads<GLushort>(quad_count);
        return gl::IndexBuffer::create_static_with_data(indices);
    }
    case UnsignedInt:
    {
        static_assert(gl::size_of_data_type(UnsignedInt) == sizeof(GLuint));
        auto indices = generate_indices_for_quads<GLuint>(quad_count);
        return gl::IndexBuffer::create_static_with_data(indices);
    }
    }

    ZTH_ASSERT(false);
    return gl::IndexBuffer::create_static_with_size(0, gl::DataType::None);
}

auto create_dynamic_index_buffer_for_quads(usize quad_count, gl::DataType indexing_data_type,
                                           gl::BufferUsage buffer_usage) -> gl::IndexBuffer
{
    ZTH_ASSERT(gl::is_an_indexing_data_type(indexing_data_type));

    switch (indexing_data_type)
    {
        using enum gl::DataType;
    case UnsignedByte:
    {
        static_assert(gl::size_of_data_type(UnsignedByte) == sizeof(GLubyte));
        auto indices = generate_indices_for_quads<GLubyte>(quad_count);
        return gl::IndexBuffer::create_dynamic_with_data(indices, buffer_usage);
    }
    case UnsignedShort:
    {
        static_assert(gl::size_of_data_type(UnsignedShort) == sizeof(GLushort));
        auto indices = generate_indices_for_quads<GLushort>(quad_count);
        return gl::IndexBuffer::create_dynamic_with_data(indices, buffer_usage);
    }
    case UnsignedInt:
    {
        static_assert(gl::size_of_data_type(UnsignedInt) == sizeof(GLuint));
        auto indices = generate_indices_for_quads<GLuint>(quad_count);
        return gl::IndexBuffer::create_dynamic_with_data(indices, buffer_usage);
    }
    }

    ZTH_ASSERT(false);
    return gl::IndexBuffer::create_dynamic(gl::DataType::None, buffer_usage);
}

} // namespace zth
