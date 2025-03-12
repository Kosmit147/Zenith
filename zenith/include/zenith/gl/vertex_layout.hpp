#pragma once

#include <glad/glad.h>

#include <initializer_list>

#include "zenith/core/typedefs.hpp"
#include "zenith/stl/vector.hpp"

namespace zth::gl {

enum class VertexLayoutElement : u8
{
    // @todo: Add all remaining types.

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
    static constexpr usize max_element_count = 16;

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
    InPlaceVector<VertexLayoutElement, max_element_count> _elements;
};

[[nodiscard]] auto get_vertex_layout_element_info(VertexLayoutElement elem) -> VertexLayoutElementInfo;

} // namespace zth::gl

#include "vertex_layout.inl"
