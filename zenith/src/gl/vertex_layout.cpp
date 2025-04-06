#include "zenith/gl/vertex_layout.hpp"

#include "zenith/core/assert.hpp"

namespace zth::gl {

auto get_vertex_layout_element_info(VertexLayoutElement elem) -> VertexLayoutElementInfo
{
    switch (elem)
    {
        using enum VertexLayoutElement;
    case Float:
        return { .count = 1,
                 .type = DataType::Float,
                 .size_bytes = static_cast<u32>(size_of_data_type(DataType::Float)) };
    case Vec2:
        return { .count = 2,
                 .type = DataType::Float,
                 .size_bytes = static_cast<u32>(size_of_data_type(DataType::Float)) * 2 };
    case Vec3:
        return { .count = 3,
                 .type = DataType::Float,
                 .size_bytes = static_cast<u32>(size_of_data_type(DataType::Float)) * 3 };
    case Vec4:
        return { .count = 4,
                 .type = DataType::Float,
                 .size_bytes = static_cast<u32>(size_of_data_type(DataType::Float)) * 4 };
    case Mat3:
        return { .count = 3,
                 .type = DataType::Float,
                 .size_bytes = static_cast<u32>(size_of_data_type(DataType::Float)) * 3,
                 .slots_occupied = 3 };
    case Mat4:
        return { .count = 4,
                 .type = DataType::Float,
                 .size_bytes = static_cast<u32>(size_of_data_type(DataType::Float)) * 4,
                 .slots_occupied = 4 };
    }

    ZTH_ASSERT(false);
    return { .count = 0, .type = DataType::Float, .size_bytes = 0, .slots_occupied = 0 };
}

} // namespace zth::gl
