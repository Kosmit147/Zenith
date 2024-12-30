#pragma once

#include <glad/glad.h>

#include <initializer_list>
#include <utility>
#include <vector>

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Core/Typedefs.hpp"

namespace zth {

enum class VertexLayoutElement : u8
{
    Float,

    Vec2,
    Vec3,
    Vec4,

    Mat3,
    Mat4,
};

struct VertexLayoutElementInfo
{
    GLint count;
    GLenum type;
    GLuint size_bytes;
    GLuint slots_occupied = 1;
};

class VertexLayout
{
public:
    constexpr VertexLayout() = default;
    constexpr VertexLayout(std::initializer_list<VertexLayoutElement> elems);
    template<typename VertexType> [[nodiscard]] constexpr static auto from_vertex() -> VertexLayout;

    constexpr auto push(VertexLayoutElement elem) -> void;
    constexpr auto clear() -> void;

    [[nodiscard]] constexpr auto size() const { return _elements.size(); }

    [[nodiscard]] constexpr auto begin() const { return _elements.begin(); }
    [[nodiscard]] constexpr auto end() const { return _elements.end(); }

    [[nodiscard]] constexpr auto begin() { return _elements.begin(); }
    [[nodiscard]] constexpr auto end() { return _elements.end(); }

private:
    std::vector<VertexLayoutElement> _elements;
};

[[nodiscard]] constexpr auto get_vertex_layout_element_info(VertexLayoutElement elem) -> VertexLayoutElementInfo
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

} // namespace zth

#include "VertexLayout.inl"
