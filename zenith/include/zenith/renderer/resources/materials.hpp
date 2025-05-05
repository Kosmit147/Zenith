#pragma once

#include <array>
#include <memory>

#include "zenith/core/typedefs.hpp"
#include "zenith/renderer/fwd.hpp"

namespace zth::materials {

constexpr inline usize plain_material_index = 0;
constexpr inline usize emerald_material_index = 1;
constexpr inline usize jade_material_index = 2;
constexpr inline usize obsidian_material_index = 3;
constexpr inline usize pearl_material_index = 4;
constexpr inline usize ruby_material_index = 5;
constexpr inline usize turquoise_material_index = 6;
constexpr inline usize brass_material_index = 7;
constexpr inline usize bronze_material_index = 8;
constexpr inline usize chrome_material_index = 9;
constexpr inline usize copper_material_index = 10;
constexpr inline usize gold_material_index = 11;
constexpr inline usize silver_material_index = 12;
constexpr inline usize black_plastic_material_index = 13;
constexpr inline usize cyan_plastic_material_index = 14;
constexpr inline usize green_plastic_material_index = 15;
constexpr inline usize red_plastic_material_index = 16;
constexpr inline usize white_plastic_material_index = 17;
constexpr inline usize yellow_plastic_material_index = 18;
constexpr inline usize black_rubber_material_index = 19;
constexpr inline usize cyan_rubber_material_index = 20;
constexpr inline usize green_rubber_material_index = 21;
constexpr inline usize red_rubber_material_index = 22;
constexpr inline usize white_rubber_material_index = 23;
constexpr inline usize yellow_rubber_material_index = 24;

using MaterialsArray = std::array<std::shared_ptr<const Material>, yellow_rubber_material_index + 1>;

auto load() -> void;
auto unload() -> void;

[[nodiscard]] auto all() -> const MaterialsArray&;

[[nodiscard]] auto plain() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto emerald() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto jade() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto obsidian() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto pearl() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto ruby() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto turquoise() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto brass() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto bronze() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto chrome() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto copper() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto gold() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto silver() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto black_plastic() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto cyan_plastic() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto green_plastic() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto red_plastic() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto white_plastic() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto yellow_plastic() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto black_rubber() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto cyan_rubber() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto green_rubber() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto red_rubber() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto white_rubber() -> const std::shared_ptr<const Material>&;
[[nodiscard]] auto yellow_rubber() -> const std::shared_ptr<const Material>&;

} // namespace zth::materials
