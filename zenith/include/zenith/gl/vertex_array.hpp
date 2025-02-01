#pragma once

#include <glad/glad.h>

#include <ranges>

#include "zenith/gl/fwd.hpp"
#include "zenith/gl/vertex_layout.hpp"

namespace zth::gl {

struct VertexArrayLayout
{
    VertexLayout vertex_buffer_layout{};
    VertexLayout instance_buffer_layout{};

    template<std::ranges::contiguous_range VertexData>
    [[nodiscard]] constexpr static auto from_vertex_data() -> VertexArrayLayout;
};

class VertexArray
{
public:
    using VertexArrayId = GLuint;

    static constexpr GLuint vertex_buffer_binding_index = 0;
    static constexpr GLuint instance_buffer_binding_index = 1;

public:
    explicit VertexArray();
    explicit VertexArray(const VertexArrayLayout& layout);

    explicit VertexArray(const VertexArrayLayout& layout, const VertexBuffer& vertex_buffer,
                         const IndexBuffer& index_buffer);
    explicit VertexArray(const VertexArrayLayout& layout, const VertexBuffer& vertex_buffer,
                         const IndexBuffer& index_buffer, const InstanceBuffer& instance_buffer);

    explicit VertexArray(const VertexArrayLayout&, VertexBuffer&&, IndexBuffer&&) = delete;
    explicit VertexArray(const VertexArrayLayout&, VertexBuffer&&, IndexBuffer&&, InstanceBuffer&&) = delete;

    VertexArray(const VertexArray& other);
    auto operator=(const VertexArray& other) -> VertexArray&;

    VertexArray(VertexArray&& other) noexcept;
    auto operator=(VertexArray&& other) noexcept -> VertexArray&;

    ~VertexArray();

    auto bind() const -> void;
    static auto unbind() -> void;

    auto bind_vertex_buffer(const VertexBuffer& vertex_buffer) -> void;
    auto bind_index_buffer(const IndexBuffer& index_buffer) -> void;
    auto bind_instance_buffer(const InstanceBuffer& instance_buffer) -> void;

    auto bind_vertex_buffer(VertexBuffer&&) = delete;
    auto bind_index_buffer(IndexBuffer&&) = delete;
    auto bind_instance_buffer(InstanceBuffer&&) = delete;

    auto bind_layout(const VertexArrayLayout& layout) -> void;

    auto unbind_vertex_buffer() -> void;
    auto unbind_index_buffer() -> void;
    auto unbind_instance_buffer() -> void;
    auto unbind_all_buffers() -> void;

    [[nodiscard]] auto native_handle() const { return _id; }
    [[nodiscard]] auto count() const -> GLsizei;
    [[nodiscard]] auto index_type() const -> GLenum;

    [[nodiscard]] auto vertex_buffer() const { return _vertex_buffer; }
    [[nodiscard]] auto index_buffer() const { return _index_buffer; }
    [[nodiscard]] auto instance_buffer() const { return _instance_buffer; }

    [[nodiscard]] auto layout() const -> auto& { return _layout; }

private:
    VertexArrayId _id = GL_NONE;

    const VertexBuffer* _vertex_buffer = nullptr;
    const IndexBuffer* _index_buffer = nullptr;
    const InstanceBuffer* _instance_buffer = nullptr;

    VertexArrayLayout _layout{};

private:
    auto create() -> void;
    auto destroy() const -> void;

    auto bind_vertex_buffer_layout() const -> void;
    auto bind_instance_buffer_layout() const -> void;
};

template<std::ranges::contiguous_range VertexData>
[[nodiscard]] constexpr auto VertexArrayLayout::from_vertex_data() -> VertexArrayLayout
{
    using VertexType = std::ranges::range_value_t<VertexData>;
    return VertexArrayLayout{ .vertex_buffer_layout = VertexLayout::from_vertex<VertexType>(),
                              .instance_buffer_layout = {} };
}

} // namespace zth::gl
