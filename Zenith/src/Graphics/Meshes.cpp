#include "Zenith/Graphics/Meshes.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Zenith/Logging/Logger.hpp"

namespace zth::meshes {

namespace {

struct CubeVertex
{
    glm::vec3 local_position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

constexpr std::array cube_vertices = {
    // front wall
    CubeVertex{ .local_position = { -0.5f, -0.5f, 0.5f }, .normal = { 0.0f, 0.0f, 1.0f }, .tex_coords = { 0.0f, 0.0f } },
    CubeVertex{ .local_position = { 0.5f, -0.5f, 0.5f }, .normal = { 0.0f, 0.0f, 1.0f }, .tex_coords = { 1.0f, 0.0f } },
    CubeVertex{ .local_position = { 0.5f, 0.5f, 0.5f }, .normal = { 0.0f, 0.0f, 1.0f }, .tex_coords = { 1.0f, 1.0f } },
    CubeVertex{ .local_position = { -0.5f, 0.5f, 0.5f }, .normal = { 0.0f, 0.0f, 1.0f }, .tex_coords = { 0.0f, 1.0f } },

    // back wall
    CubeVertex{ .local_position = { -0.5f, -0.5f, -0.5f }, .normal = { 0.0f, 0.0f, -1.0f }, .tex_coords = { 0.0f, 0.0f } },
    CubeVertex{ .local_position = { -0.5f, 0.5f, -0.5f }, .normal = { 0.0f, 0.0f, -1.0f }, .tex_coords = { 0.0f, 1.0f } },
    CubeVertex{ .local_position = { 0.5f, 0.5f, -0.5f }, .normal = { 0.0f, 0.0f, -1.0f }, .tex_coords = { 1.0f, 1.0f } },
    CubeVertex{ .local_position = { 0.5f, -0.5f, -0.5f }, .normal = { 0.0f, 0.0f, -1.0f }, .tex_coords = { 1.0f, 0.0f } },
                                                                     
    // left wall
    CubeVertex{ .local_position = { -0.5f, 0.5f, 0.5f }, .normal = { -1.0f, 0.0f, 0.0f }, .tex_coords = { 1.0f, 0.0f } },
    CubeVertex{ .local_position = { -0.5f, 0.5f, -0.5f }, .normal = { -1.0f, 0.0f, 0.0f }, .tex_coords = { 1.0f, 1.0f } },
    CubeVertex{ .local_position = { -0.5f, -0.5f, -0.5f }, .normal = { -1.0f, 0.0f, 0.0f }, .tex_coords = { 0.0f, 1.0f } },
    CubeVertex{ .local_position = { -0.5f, -0.5f, 0.5f }, .normal = { -1.0f, 0.0f, 0.0f }, .tex_coords = { 0.0f, 0.0f } },
                                                                    
    // right wall
    CubeVertex{ .local_position = { 0.5f, 0.5f, 0.5f }, .normal = { 1.0f, 0.0f, 0.0f }, .tex_coords = { 1.0f, 0.0f } },
    CubeVertex{ .local_position = { 0.5f, -0.5f, 0.5f }, .normal = { 1.0f, 0.0f, 0.0f }, .tex_coords = { 0.0f, 0.0f } },
    CubeVertex{ .local_position = { 0.5f, -0.5f, -0.5f }, .normal = { 1.0f, 0.0f, 0.0f }, .tex_coords = { 0.0f, 1.0f } },
    CubeVertex{ .local_position = { 0.5f, 0.5f, -0.5f }, .normal = { 1.0f, 0.0f, 0.0f }, .tex_coords = { 1.0f, 1.0f } },
                                                                     
    // bottom wall
    CubeVertex{ .local_position = { -0.5f, -0.5f, -0.5f }, .normal = { 0.0f, -1.0f, 0.0f }, .tex_coords = { 0.0f, 1.0f } },
    CubeVertex{ .local_position = { 0.5f, -0.5f, -0.5f }, .normal = { 0.0f, -1.0f, 0.0f }, .tex_coords = { 1.0f, 1.0f } },
    CubeVertex{ .local_position = { 0.5f, -0.5f, 0.5f }, .normal = { 0.0f, -1.0f, 0.0f }, .tex_coords = { 1.0f, 0.0f } },
    CubeVertex{ .local_position = { -0.5f, -0.5f, 0.5f }, .normal = { 0.0f, -1.0f, 0.0f }, .tex_coords = { 0.0f, 0.0f } },
                                                                     
    // top wall
    CubeVertex{ .local_position = { -0.5f, 0.5f, -0.5f }, .normal = { 0.0f, 1.0f, 0.0f }, .tex_coords = { 0.0f, 1.0f } },
    CubeVertex{ .local_position = { -0.5f, 0.5f, 0.5f }, .normal = { 0.0f, 1.0f, 0.0f }, .tex_coords = { 0.0f, 0.0f } },
    CubeVertex{ .local_position = { 0.5f, 0.5f, 0.5f }, .normal = { 0.0f, 1.0f, 0.0f }, .tex_coords = { 1.0f, 0.0f } },
    CubeVertex{ .local_position = { 0.5f, 0.5f, -0.5f }, .normal = { 0.0f, 1.0f, 0.0f }, .tex_coords = { 1.0f, 1.0f } },
};

// clang-format off
constexpr std::array<GLushort, 36> cube_indices = {
    // front wall
    0, 1, 2,
    0, 2, 3,

    // back wall
    4, 5, 6,
    4, 6, 7,

    // left wall
    8, 9, 10,
    8, 10, 11,

    // right wall
    12, 13, 14,
    12, 14, 15,

    // bottom wall
    16, 17, 18,
    16, 18, 19,

    // top wall
    20, 21, 22,
    20, 22, 23,
};
// clang-format on

std::optional<Mesh> cube_mesh_local;

} // namespace

auto load_meshes() -> void
{
    cube_mesh_local.emplace(cube_vertices, cube_indices);
    cube_mesh = &cube_mesh_local.value();

    ZTH_CORE_INFO("Meshes loaded.");
}

auto unload_meshes() -> void
{
    cube_mesh_local.reset();
    cube_mesh = nullptr;

    ZTH_CORE_INFO("Meshes unloaded.");
}

} // namespace zth::meshes
