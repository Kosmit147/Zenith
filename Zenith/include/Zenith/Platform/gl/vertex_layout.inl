#pragma once

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <utility>

#include "zenith/core/assert.hpp"
#include "zenith/utility/struct_arity.hpp"

namespace zth {

template<typename T> constexpr auto to_vertex_layout_elem() -> VertexLayoutElement
{
    static_assert(false, "not implemented");
    return static_cast<VertexLayoutElement>(NULL);
}

template<> constexpr auto to_vertex_layout_elem<const GLfloat>() -> VertexLayoutElement
{
    return VertexLayoutElement::Float;
}

template<> constexpr auto to_vertex_layout_elem<const glm::vec2>() -> VertexLayoutElement
{
    return VertexLayoutElement::Vec2;
}

template<> constexpr auto to_vertex_layout_elem<const glm::vec3>() -> VertexLayoutElement
{
    return VertexLayoutElement::Vec3;
}

template<> constexpr auto to_vertex_layout_elem<const glm::vec4>() -> VertexLayoutElement
{
    return VertexLayoutElement::Vec4;
}

template<> constexpr auto to_vertex_layout_elem<const glm::mat3>() -> VertexLayoutElement
{
    return VertexLayoutElement::Mat3;
}

template<> constexpr auto to_vertex_layout_elem<const glm::mat4>() -> VertexLayoutElement
{
    return VertexLayoutElement::Mat4;
}

constexpr VertexLayout::VertexLayout(std::initializer_list<VertexLayoutElement> elems) : _elements(elems) {}

constexpr auto VertexLayout::push(VertexLayoutElement elem) -> void
{
    _elements.push_back(elem);
}

constexpr auto VertexLayout::clear() -> void
{
    _elements.clear();
}

template<typename VertexType> constexpr auto VertexLayout::from_vertex() -> VertexLayout
{
    constexpr auto dummy_vertex = VertexType{};
    constexpr auto arity = get_struct_arity<VertexType>();

    if constexpr (arity == 0)
    {
        return {};
    }
    else if constexpr (arity == 1)
    {
        [[maybe_unused]] auto& [e1] = dummy_vertex;
        return { to_vertex_layout_elem<decltype(e1)>() };
    }
    else if constexpr (arity == 2)
    {
        [[maybe_unused]] auto& [e1, e2] = dummy_vertex;
        return { to_vertex_layout_elem<decltype(e1)>(), to_vertex_layout_elem<decltype(e2)>() };
    }
    else if constexpr (arity == 3)
    {
        [[maybe_unused]] auto& [e1, e2, e3] = dummy_vertex;
        return { to_vertex_layout_elem<decltype(e1)>(), to_vertex_layout_elem<decltype(e2)>(),
                 to_vertex_layout_elem<decltype(e3)>() };
    }
    else if constexpr (arity == 4)
    {
        [[maybe_unused]] auto& [e1, e2, e3, e4] = dummy_vertex;
        return { to_vertex_layout_elem<decltype(e1)>(), to_vertex_layout_elem<decltype(e2)>(),
                 to_vertex_layout_elem<decltype(e3)>(), to_vertex_layout_elem<decltype(e4)>() };
    }
    else if constexpr (arity == 5)
    {
        [[maybe_unused]] auto& [e1, e2, e3, e4, e5] = dummy_vertex;
        return { to_vertex_layout_elem<decltype(e1)>(), to_vertex_layout_elem<decltype(e2)>(),
                 to_vertex_layout_elem<decltype(e3)>(), to_vertex_layout_elem<decltype(e4)>(),
                 to_vertex_layout_elem<decltype(e5)>() };
    }
    else if constexpr (arity == 6)
    {
        [[maybe_unused]] auto& [e1, e2, e3, e4, e5, e6] = dummy_vertex;
        return { to_vertex_layout_elem<decltype(e1)>(), to_vertex_layout_elem<decltype(e2)>(),
                 to_vertex_layout_elem<decltype(e3)>(), to_vertex_layout_elem<decltype(e4)>(),
                 to_vertex_layout_elem<decltype(e5)>(), to_vertex_layout_elem<decltype(e6)>() };
    }
    else if constexpr (arity == 7)
    {
        [[maybe_unused]] auto& [e1, e2, e3, e4, e5, e6, e7] = dummy_vertex;
        return { to_vertex_layout_elem<decltype(e1)>(), to_vertex_layout_elem<decltype(e2)>(),
                 to_vertex_layout_elem<decltype(e3)>(), to_vertex_layout_elem<decltype(e4)>(),
                 to_vertex_layout_elem<decltype(e5)>(), to_vertex_layout_elem<decltype(e6)>(),
                 to_vertex_layout_elem<decltype(e7)>() };
    }
    else if constexpr (arity == 8)
    {
        [[maybe_unused]] auto& [e1, e2, e3, e4, e5, e6, e7, e8] = dummy_vertex;
        return { to_vertex_layout_elem<decltype(e1)>(), to_vertex_layout_elem<decltype(e2)>(),
                 to_vertex_layout_elem<decltype(e3)>(), to_vertex_layout_elem<decltype(e4)>(),
                 to_vertex_layout_elem<decltype(e5)>(), to_vertex_layout_elem<decltype(e6)>(),
                 to_vertex_layout_elem<decltype(e7)>(), to_vertex_layout_elem<decltype(e8)>() };
    }
    else if constexpr (arity == 9)
    {
        [[maybe_unused]] auto& [e1, e2, e3, e4, e5, e6, e7, e8, e9] = dummy_vertex;
        return { to_vertex_layout_elem<decltype(e1)>(), to_vertex_layout_elem<decltype(e2)>(),
                 to_vertex_layout_elem<decltype(e3)>(), to_vertex_layout_elem<decltype(e4)>(),
                 to_vertex_layout_elem<decltype(e5)>(), to_vertex_layout_elem<decltype(e6)>(),
                 to_vertex_layout_elem<decltype(e7)>(), to_vertex_layout_elem<decltype(e8)>(),
                 to_vertex_layout_elem<decltype(e9)>() };
    }
    else if constexpr (arity == 10)
    {
        [[maybe_unused]] auto& [e1, e2, e3, e4, e5, e6, e7, e8, e9, e10] = dummy_vertex;
        return { to_vertex_layout_elem<decltype(e1)>(), to_vertex_layout_elem<decltype(e2)>(),
                 to_vertex_layout_elem<decltype(e3)>(), to_vertex_layout_elem<decltype(e4)>(),
                 to_vertex_layout_elem<decltype(e5)>(), to_vertex_layout_elem<decltype(e6)>(),
                 to_vertex_layout_elem<decltype(e7)>(), to_vertex_layout_elem<decltype(e8)>(),
                 to_vertex_layout_elem<decltype(e9)>(), to_vertex_layout_elem<decltype(e10)>() };
    }
    else
    {
        static_assert(false, "not implemented");
        return {};
    }
}

constexpr auto get_vertex_layout_element_info(VertexLayoutElement elem) -> VertexLayoutElementInfo
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
