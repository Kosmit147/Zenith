#pragma once

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "zenith/util/reflection.hpp"

namespace zth::gl {

template<typename T> constexpr inline VertexLayoutElement to_vertex_layout_elem; // Must be specialized.

template<> constexpr inline auto to_vertex_layout_elem<GLfloat> = VertexLayoutElement::Float;
template<> constexpr inline auto to_vertex_layout_elem<const GLfloat> = VertexLayoutElement::Float;
template<> constexpr inline auto to_vertex_layout_elem<glm::vec2> = VertexLayoutElement::Vec2;
template<> constexpr inline auto to_vertex_layout_elem<const glm::vec2> = VertexLayoutElement::Vec2;
template<> constexpr inline auto to_vertex_layout_elem<glm::vec3> = VertexLayoutElement::Vec3;
template<> constexpr inline auto to_vertex_layout_elem<const glm::vec3> = VertexLayoutElement::Vec3;
template<> constexpr inline auto to_vertex_layout_elem<glm::vec4> = VertexLayoutElement::Vec4;
template<> constexpr inline auto to_vertex_layout_elem<const glm::vec4> = VertexLayoutElement::Vec4;
template<> constexpr inline auto to_vertex_layout_elem<glm::mat3> = VertexLayoutElement::Mat3;
template<> constexpr inline auto to_vertex_layout_elem<const glm::mat3> = VertexLayoutElement::Mat3;
template<> constexpr inline auto to_vertex_layout_elem<glm::mat4> = VertexLayoutElement::Mat4;
template<> constexpr inline auto to_vertex_layout_elem<const glm::mat4> = VertexLayoutElement::Mat4;

constexpr VertexLayout::VertexLayout(std::initializer_list<VertexLayoutElement> elements, u32 stride_bytes)
    : _elements{ elements }, _stride_bytes{ stride_bytes }
{}

template<typename VertexType> constexpr auto VertexLayout::derive_from_vertex() -> VertexLayout
{
    // @refactor: Use in-built reflection once we're on C++26.

    static constexpr auto dummy_vertex = VertexType{};
    static constexpr auto arity = meta::struct_arity<VertexType>();

    VertexLayout result;
    result.set_stride(sizeof(VertexType));

    if constexpr (arity == 0)
    {
        static_assert(false, "empty vertex");
    }
    else if constexpr (arity == 1)
    {
        [[maybe_unused]] auto& [e1] = dummy_vertex;
        result.push(to_vertex_layout_elem<decltype(e1)>);
    }
    else if constexpr (arity == 2)
    {
        [[maybe_unused]] auto& [e1, e2] = dummy_vertex;
        result.push(to_vertex_layout_elem<decltype(e1)>);
        result.push(to_vertex_layout_elem<decltype(e2)>);
    }
    else if constexpr (arity == 3)
    {
        [[maybe_unused]] auto& [e1, e2, e3] = dummy_vertex;
        result.push(to_vertex_layout_elem<decltype(e1)>);
        result.push(to_vertex_layout_elem<decltype(e2)>);
        result.push(to_vertex_layout_elem<decltype(e3)>);
    }
    else if constexpr (arity == 4)
    {
        [[maybe_unused]] auto& [e1, e2, e3, e4] = dummy_vertex;
        result.push(to_vertex_layout_elem<decltype(e1)>);
        result.push(to_vertex_layout_elem<decltype(e2)>);
        result.push(to_vertex_layout_elem<decltype(e3)>);
        result.push(to_vertex_layout_elem<decltype(e4)>);
    }
    else if constexpr (arity == 5)
    {
        [[maybe_unused]] auto& [e1, e2, e3, e4, e5] = dummy_vertex;
        result.push(to_vertex_layout_elem<decltype(e1)>);
        result.push(to_vertex_layout_elem<decltype(e2)>);
        result.push(to_vertex_layout_elem<decltype(e3)>);
        result.push(to_vertex_layout_elem<decltype(e4)>);
        result.push(to_vertex_layout_elem<decltype(e5)>);
    }
    else if constexpr (arity == 6)
    {
        [[maybe_unused]] auto& [e1, e2, e3, e4, e5, e6] = dummy_vertex;
        result.push(to_vertex_layout_elem<decltype(e1)>);
        result.push(to_vertex_layout_elem<decltype(e2)>);
        result.push(to_vertex_layout_elem<decltype(e3)>);
        result.push(to_vertex_layout_elem<decltype(e4)>);
        result.push(to_vertex_layout_elem<decltype(e5)>);
        result.push(to_vertex_layout_elem<decltype(e6)>);
    }
    else if constexpr (arity == 7)
    {
        [[maybe_unused]] auto& [e1, e2, e3, e4, e5, e6, e7] = dummy_vertex;
        result.push(to_vertex_layout_elem<decltype(e1)>);
        result.push(to_vertex_layout_elem<decltype(e2)>);
        result.push(to_vertex_layout_elem<decltype(e3)>);
        result.push(to_vertex_layout_elem<decltype(e4)>);
        result.push(to_vertex_layout_elem<decltype(e5)>);
        result.push(to_vertex_layout_elem<decltype(e6)>);
        result.push(to_vertex_layout_elem<decltype(e7)>);
    }
    else if constexpr (arity == 8)
    {
        [[maybe_unused]] auto& [e1, e2, e3, e4, e5, e6, e7, e8] = dummy_vertex;
        result.push(to_vertex_layout_elem<decltype(e1)>);
        result.push(to_vertex_layout_elem<decltype(e2)>);
        result.push(to_vertex_layout_elem<decltype(e3)>);
        result.push(to_vertex_layout_elem<decltype(e4)>);
        result.push(to_vertex_layout_elem<decltype(e5)>);
        result.push(to_vertex_layout_elem<decltype(e6)>);
        result.push(to_vertex_layout_elem<decltype(e7)>);
        result.push(to_vertex_layout_elem<decltype(e8)>);
    }
    else if constexpr (arity == 9)
    {
        [[maybe_unused]] auto& [e1, e2, e3, e4, e5, e6, e7, e8, e9] = dummy_vertex;
        result.push(to_vertex_layout_elem<decltype(e1)>);
        result.push(to_vertex_layout_elem<decltype(e2)>);
        result.push(to_vertex_layout_elem<decltype(e3)>);
        result.push(to_vertex_layout_elem<decltype(e4)>);
        result.push(to_vertex_layout_elem<decltype(e5)>);
        result.push(to_vertex_layout_elem<decltype(e6)>);
        result.push(to_vertex_layout_elem<decltype(e7)>);
        result.push(to_vertex_layout_elem<decltype(e8)>);
        result.push(to_vertex_layout_elem<decltype(e9)>);
    }
    else if constexpr (arity == 10)
    {
        [[maybe_unused]] auto& [e1, e2, e3, e4, e5, e6, e7, e8, e9, e10] = dummy_vertex;
        result.push(to_vertex_layout_elem<decltype(e1)>);
        result.push(to_vertex_layout_elem<decltype(e2)>);
        result.push(to_vertex_layout_elem<decltype(e3)>);
        result.push(to_vertex_layout_elem<decltype(e4)>);
        result.push(to_vertex_layout_elem<decltype(e5)>);
        result.push(to_vertex_layout_elem<decltype(e6)>);
        result.push(to_vertex_layout_elem<decltype(e7)>);
        result.push(to_vertex_layout_elem<decltype(e8)>);
        result.push(to_vertex_layout_elem<decltype(e9)>);
        result.push(to_vertex_layout_elem<decltype(e10)>);
    }
    else
    {
        static_assert(false, "not implemented");
    }

    return result;
}

constexpr auto VertexLayout::push(VertexLayoutElement elem) -> VertexLayoutElement&
{
    return _elements.push_back(elem);
}

constexpr auto VertexLayout::set_stride(u32 stride_bytes) -> void
{
    _stride_bytes = stride_bytes;
}

constexpr auto VertexLayout::clear() -> void
{
    _elements.clear();
}

} // namespace zth::gl
