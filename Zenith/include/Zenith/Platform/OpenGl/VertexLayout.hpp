#pragma once

#include <glad/glad.h>

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
};

struct VertexLayoutElementInfo
{
    GLint count;
    GLenum type;
    GLuint size_bytes;
};

class VertexLayout
{
public:
    constexpr VertexLayout() = default;
    constexpr VertexLayout(std::initializer_list<VertexLayoutElement> elems) : _elements(elems) {}
    template<typename VertexType> [[nodiscard]] constexpr static auto from_vertex() -> VertexLayout;

    auto push(VertexLayoutElement elem) -> void { _elements.push_back(elem); }
    auto clear() -> void { _elements.clear(); }

    [[nodiscard]] auto size() const { return _elements.size(); }

    [[nodiscard]] auto begin() const { return _elements.begin(); }
    [[nodiscard]] auto end() const { return _elements.end(); }

    [[nodiscard]] auto begin() { return _elements.begin(); }
    [[nodiscard]] auto end() { return _elements.end(); }

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
    }

    ZTH_ASSERT(false);
    std::unreachable();
}

} // namespace zth

#include "VertexLayout.inl"
