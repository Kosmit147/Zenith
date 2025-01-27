#pragma once

#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "zenith/gl/vertex_array.hpp"
#include "zenith/gl/vertex_layout.hpp"

namespace zth {

struct InstanceVertex
{
    glm::vec3 transform_col_0;
    glm::vec3 transform_col_1;
    glm::vec3 transform_col_2;
    glm::vec3 transform_col_3;

    glm::mat3 normal_mat;

    static const VertexLayout layout;
};

inline const VertexLayout InstanceVertex::layout = VertexLayout::from_vertex<InstanceVertex>();

struct StandardVertex
{
    glm::vec3 local_position;
    glm::vec3 normal;
    glm::vec2 tex_coords;

    static const VertexLayout layout;
    static const VertexArrayLayout instanced_layout;
};

inline const VertexLayout StandardVertex::layout = VertexLayout::from_vertex<StandardVertex>();

inline const VertexArrayLayout StandardVertex::instanced_layout = {
    .vertex_buffer_layout = StandardVertex::layout,
    .instance_buffer_layout = InstanceVertex::layout,
};

} // namespace zth
