#include "Zenith/Graphics/Meshes.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace zth::meshes {

namespace {

struct CubeVertex
{
    glm::vec3 local_position;
    glm::vec2 tex_coords;
};

constexpr std::array cube_vertices = {
    // 0
    CubeVertex{ .local_position = { -1.0f, -1.0f, 1.0f }, .tex_coords = { 0.0f, 1.0f } },
    // 1
    CubeVertex{ .local_position = { -1.0f, -1.0f, -1.0f }, .tex_coords = { 1.0f, 1.0f } },
    // 2
    CubeVertex{ .local_position = { 1.0f, -1.0f, -1.0f }, .tex_coords = { 1.0f, 0.0f } },
    // 3
    CubeVertex{ .local_position = { 1.0f, -1.0f, 1.0f }, .tex_coords = { 0.0f, 0.0f } },
    // 4
    CubeVertex{ .local_position = { -1.0f, 1.0f, 1.0f }, .tex_coords = { 1.0f, 0.0f } },
    // 5
    CubeVertex{ .local_position = { -1.0f, 1.0f, -1.0f }, .tex_coords = { 0.0f, 0.0f } },
    // 6
    CubeVertex{ .local_position = { 1.0f, 1.0f, -1.0f }, .tex_coords = { 0.0f, 1.0f } },
    // 7
    CubeVertex{ .local_position = { 1.0f, 1.0f, 1.0f }, .tex_coords = { 1.0f, 1.0f } },
};

// clang-format off
constexpr std::array<GLushort, 36> cube_indices = {
    // bottom wall
    0, 1, 2,
    2, 3, 0,

    // top wall
    4, 5, 6,
    6, 7, 4,

    // right wall
    3, 7, 6,
    6, 2, 3,

    // front wall
    0, 4, 7,
    7, 3, 0,

    // left wall
    1, 0, 4,
    4, 5, 1,

    // back wall
    1, 2, 6,
    6, 5, 1,
};
// clang-format on

std::optional<Mesh> cube_mesh_local;

} // namespace

auto load_meshes() -> void
{
    cube_mesh_local.emplace(cube_vertices, BufferUsage::static_draw, cube_indices, BufferUsage::static_draw);
    cube_mesh = &cube_mesh_local.value();
}

auto unload_meshes() -> void
{
    cube_mesh_local.reset();
    cube_mesh = nullptr;
}

} // namespace zth::meshes
