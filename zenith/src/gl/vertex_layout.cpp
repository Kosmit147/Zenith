#include "zenith/gl/vertex_layout.hpp"

#include "zenith/core/assert.hpp"

namespace zth::gl {

auto get_vertex_layout_element_info(VertexLayoutElement elem) -> VertexLayoutElementInfo
{
    switch (elem)
    {
        using enum VertexLayoutElement;
    case Float:
        return { .count = 1, .type = GL_FLOAT, .size_bytes = sizeof(GLfloat) };
    case Vec2:
        return { .count = 2, .type = GL_FLOAT, .size_bytes = sizeof(GLfloat) * 2 };
    case Vec3:
        return { .count = 3, .type = GL_FLOAT, .size_bytes = sizeof(GLfloat) * 3 };
    case Vec4:
        return { .count = 4, .type = GL_FLOAT, .size_bytes = sizeof(GLfloat) * 4 };
    case Mat3:
        return { .count = 3, .type = GL_FLOAT, .size_bytes = sizeof(GLfloat) * 3, .slots_occupied = 3 };
    case Mat4:
        return { .count = 4, .type = GL_FLOAT, .size_bytes = sizeof(GLfloat) * 4, .slots_occupied = 4 };
    }

    ZTH_ASSERT(false);
    std::unreachable();
}

} // namespace zth::gl
