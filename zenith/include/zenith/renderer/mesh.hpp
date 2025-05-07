#pragma once

#include <span>

#include "zenith/core/typedefs.hpp"
#include "zenith/gl/buffer.hpp"
#include "zenith/gl/util.hpp"
#include "zenith/gl/vertex_array.hpp"
#include "zenith/stl/vector.hpp"
#include "zenith/util/macros.hpp"

namespace zth {

// Every mesh's vertex array gets implicitly bound to the renderer's instance buffer.
class Mesh
{
public:
    explicit Mesh() = default;
    ZTH_DEFAULT_COPY_DEFAULT_MOVE(Mesh)
    virtual ~Mesh() = default;

    [[nodiscard]] virtual auto vertex_array() -> gl::VertexArray& = 0;
    [[nodiscard]] virtual auto vertex_array() const -> const gl::VertexArray& = 0;
};

// Every mesh's vertex array gets implicitly bound to the renderer's instance buffer.
template<typename Vertex = StandardVertex, gl::IndexingType Index = u32> class IndexedMesh : public Mesh
{
public:
    explicit IndexedMesh() = default;

    explicit IndexedMesh(std::span<const Vertex> vertex_data, std::span<const Index> index_data);

    IndexedMesh(const IndexedMesh& other);
    auto operator=(const IndexedMesh& other) -> IndexedMesh&;

    IndexedMesh(IndexedMesh&& other) noexcept;
    auto operator=(IndexedMesh&& other) noexcept -> IndexedMesh&;

    ~IndexedMesh() override = default;

    [[nodiscard]] auto vertex_array() -> gl::VertexArray& override { return _vertex_array; }
    [[nodiscard]] auto vertex_array() const -> const gl::VertexArray& override { return _vertex_array; }

    [[nodiscard]] auto vertex_buffer(this auto&& self) -> auto& { return self._vertex_buffer; }
    [[nodiscard]] auto index_buffer(this auto&& self) -> auto& { return self._index_buffer; }

    [[nodiscard]] auto vertices(this auto&& self) -> auto& { return self._vertices; }
    [[nodiscard]] auto indices(this auto&& self) -> auto& { return self._indices; }

    [[nodiscard]] auto vertex_layout() const -> auto& { return _vertex_buffer.layout(); }
    [[nodiscard]] auto indexing_data_type() const -> auto& { return _index_buffer.indexing_data_type(); }

private:
    gl::VertexBuffer _vertex_buffer;
    gl::IndexBuffer _index_buffer;
    gl::VertexArray _vertex_array;

    Vector<Vertex> _vertices;
    Vector<Index> _indices;
};

// QuadMesh is a mesh made up of quads (sets of two triangles). There's no need to provide indices. The order of
// vertices for a single quad should be counter-clockwise starting from the top-left vertex. The number of vertices that
// gets drawn is the number of vertices in the vertex buffer (its size divided by its stride). Every mesh's vertex array
// gets implicitly bound to the renderer's instance buffer.
template<typename Vertex = StandardVertex> class QuadMesh : public Mesh
{
public:
    explicit QuadMesh() = default;

    explicit QuadMesh(std::span<const Vertex> vertex_data);

    QuadMesh(const QuadMesh& other);
    auto operator=(const QuadMesh& other) -> QuadMesh&;

    QuadMesh(QuadMesh&& other) noexcept;
    auto operator=(QuadMesh&& other) noexcept -> QuadMesh&;

    ~QuadMesh() override = default;

    [[nodiscard]] auto vertex_array() -> gl::VertexArray& override { return _vertex_array; }
    [[nodiscard]] auto vertex_array() const -> const gl::VertexArray& override { return _vertex_array; }

    [[nodiscard]] auto vertex_buffer(this auto&& self) -> auto& { return self._vertex_buffer; }
    [[nodiscard]] auto index_buffer() const -> const gl::IndexBuffer&;

    [[nodiscard]] auto vertices(this auto&& self) -> auto& { return self._vertices; }

    [[nodiscard]] auto vertex_layout() const -> auto& { return _vertex_buffer.layout(); }
    [[nodiscard]] auto indexing_data_type() const { return index_buffer().indexing_data_type(); }

private:
    gl::VertexBuffer _vertex_buffer;
    gl::VertexArray _vertex_array;

    Vector<Vertex> _vertices;
};

} // namespace zth

#include "mesh.inl"
