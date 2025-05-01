#pragma once

#include <array>
#include <concepts>

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/buffer.hpp"
#include "zenith/gl/util.hpp"
#include "zenith/memory/temporary_storage.hpp"
#include "zenith/stl/vector.hpp"

namespace zth {

constexpr usize vertices_per_triangle = 3;
constexpr usize vertices_per_quad = 4;

constexpr usize triangles_per_quad = 2;

constexpr usize indices_per_triangle = 3;
constexpr usize indices_per_quad = triangles_per_quad * indices_per_triangle;

constexpr std::array<usize, indices_per_quad> quad_indices = { 0, 1, 2, 0, 2, 3 };

[[nodiscard]] constexpr auto get_triangle_vertex_count_from_quad_vertex_count(usize vertex_count) -> usize
{
    static_assert(vertices_per_quad == 4);
    static_assert(indices_per_quad == 6);

    // 4 vertices per quad translate to 6 vertices per 2 triangles.
    return vertex_count + vertex_count / 2;
}

[[nodiscard]] constexpr auto max_value_of_quads_index_buffer_index(usize quad_count)
{
    return quad_count * vertices_per_quad - 1;
}

template<std::unsigned_integral IndexingDataType>
[[nodiscard]] auto generate_indices_for_quads(usize quad_count) -> TemporaryVector<IndexingDataType>
{
    static_assert(gl::is_an_indexing_data_type(gl::to_data_type<IndexingDataType>), "not a valid indexing data type");

    TemporaryVector<IndexingDataType> result;
    result.reserve(quad_count * indices_per_quad);

    usize increment = 0;

    for (usize i = 0; i < quad_count; i++)
    {
        for (auto index : quad_indices)
            result.push_back(static_cast<IndexingDataType>(index + increment));

        increment += vertices_per_quad;
    }

    return result;
}

[[nodiscard]] auto create_static_index_buffer_for_quads(usize quad_count) -> gl::IndexBuffer;
[[nodiscard]] auto create_dynamic_index_buffer_for_quads(usize quad_count,
                                                         gl::DataType indexing_data_type = gl::DataType::UnsignedInt,
                                                         gl::BufferUsage buffer_usage = gl::BufferUsage::dynamic_draw)
    -> gl::IndexBuffer;

} // namespace zth
