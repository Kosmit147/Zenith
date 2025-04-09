#pragma once

#include <initializer_list>

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/util.hpp"
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
    u32 count;
    DataType type;
    u32 size_bytes;
    u32 slots_occupied = 1;
};

class VertexLayout
{
public:
    static constexpr usize max_element_count = 16;

public:
    constexpr VertexLayout() = default;
    constexpr VertexLayout(std::initializer_list<VertexLayoutElement> elements, u32 stride_bytes);
    template<typename VertexType> [[nodiscard]] constexpr static auto derive_from_vertex() -> VertexLayout;

    constexpr auto push(VertexLayoutElement elem) -> VertexLayoutElement&;
    constexpr auto set_stride(u32 stride_bytes) -> void;
    constexpr auto clear() -> void;

    [[nodiscard]] constexpr auto count() const { return _elements.size(); }

    [[nodiscard]] constexpr auto begin(this auto&& self) -> decltype(auto) { return self._elements.begin(); }
    [[nodiscard]] constexpr auto end(this auto&& self) -> decltype(auto) { return self._elements.end(); }

    [[nodiscard]] constexpr auto stride() const { return _stride_bytes; }

private:
    InPlaceVector<VertexLayoutElement, max_element_count> _elements;
    u32 _stride_bytes = 0;
};

[[nodiscard]] auto get_vertex_layout_element_info(VertexLayoutElement elem) -> VertexLayoutElementInfo;

} // namespace zth::gl

#include "vertex_layout.inl"
