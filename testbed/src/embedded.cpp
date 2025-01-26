#include "embedded.hpp"

#include <battery/embed.hpp>

namespace embedded {

const std::span<const std::uint8_t> cobble_diffuse_map_data = b::embed<"assets/cobble_diffuse.png">().data();
const std::span<const std::uint8_t> container_diffuse_map_data = b::embed<"assets/container_diffuse.jpg">().data();
const std::span<const std::uint8_t> container2_diffuse_map_data = b::embed<"assets/container2_diffuse.png">().data();
const std::span<const std::uint8_t> container2_specular_map_data = b::embed<"assets/container2_specular.png">().data();
const std::span<const std::uint8_t> emoji_diffuse_map_data = b::embed<"assets/emoji_diffuse.png">().data();
const std::span<const std::uint8_t> matrix_emission_map_data = b::embed<"assets/matrix_emission.jpg">().data();
const std::span<const std::uint8_t> wall_diffuse_map_data = b::embed<"assets/wall_diffuse.jpg">().data();

} // namespace embedded
