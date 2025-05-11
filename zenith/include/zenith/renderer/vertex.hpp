#pragma once

#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "zenith/gl/vertex_layout.hpp"

namespace zth {

struct InstanceVertex
{
    glm::vec3 transform_col_0;
    glm::vec3 transform_col_1;
    glm::vec3 transform_col_2;
    glm::vec3 transform_col_3;

    glm::mat3 normal_mat;

    static const gl::VertexLayout layout;
};

inline const gl::VertexLayout InstanceVertex::layout = gl::VertexLayout::derive_from_vertex<InstanceVertex>();

struct StandardVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;

    static const gl::VertexLayout layout;
};

inline const gl::VertexLayout StandardVertex::layout = gl::VertexLayout::derive_from_vertex<StandardVertex>();

struct Vertex2D
{
    glm::vec2 position;
    glm::vec2 uv;
    glm::vec4 color;

    static const gl::VertexLayout layout;
};

inline const gl::VertexLayout Vertex2D::layout = gl::VertexLayout::derive_from_vertex<Vertex2D>();

} // namespace zth
