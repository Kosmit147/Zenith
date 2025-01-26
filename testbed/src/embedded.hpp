#pragma once

#include <cstdint>
#include <span>

namespace embedded {

extern const std::span<const std::uint8_t> cobble_diffuse_map_data;
extern const std::span<const std::uint8_t> container_diffuse_map_data;
extern const std::span<const std::uint8_t> container2_diffuse_map_data;
extern const std::span<const std::uint8_t> container2_specular_map_data;
extern const std::span<const std::uint8_t> emoji_diffuse_map_data;
extern const std::span<const std::uint8_t> matrix_emission_map_data;
extern const std::span<const std::uint8_t> wall_diffuse_map_data;

} // namespace embedded
