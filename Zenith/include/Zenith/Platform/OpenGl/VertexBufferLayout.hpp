#pragma once

#include <glad/glad.h>

#include <utility>
#include <vector>

#include "Zenith/Core/Assert.hpp"
#include "Zenith/Core/Typedefs.hpp"

namespace zth {

enum class VertexBufferLayoutElement : u8
{
    Float,

    Vec2,
    Vec3,
    Vec4,

    Mat3,
    Mat4,
};

struct VertexBufferLayoutElementInfo
{
    GLint count;
    GLenum type;
    GLuint size_bytes;
    GLuint slots_occupied = 1;
};

class VertexBufferLayout
{
public:
    constexpr VertexBufferLayout() = default;
    constexpr VertexBufferLayout(std::initializer_list<VertexBufferLayoutElement> elems) : _elements(elems) {}
    template<typename VertexType> [[nodiscard]] constexpr static auto from_vertex() -> VertexBufferLayout;

    auto push(VertexBufferLayoutElement elem) -> void { _elements.push_back(elem); }
    auto clear() -> void { _elements.clear(); }

    [[nodiscard]] auto size() const { return _elements.size(); }

    [[nodiscard]] auto begin() const { return _elements.begin(); }
    [[nodiscard]] auto end() const { return _elements.end(); }

    [[nodiscard]] auto begin() { return _elements.begin(); }
    [[nodiscard]] auto end() { return _elements.end(); }

private:
    std::vector<VertexBufferLayoutElement> _elements;
};

[[nodiscard]] constexpr auto
get_vertex_layout_element_info(VertexBufferLayoutElement elem) -> VertexBufferLayoutElementInfo
{
    switch (elem)
    {
        using enum VertexBufferLayoutElement;
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

#include "VertexBufferLayout.inl"
