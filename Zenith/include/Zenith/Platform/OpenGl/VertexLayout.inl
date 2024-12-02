#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Zenith/Utility/StructArity.hpp"

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
    else
    {
        static_assert(false, "not implemented");
        return {};
    }
}

} // namespace zth
