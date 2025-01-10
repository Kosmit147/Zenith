#pragma once

#include <array>

#include "Zenith/Core/Typedefs.hpp"
#include "Zenith/Graphics/Material.hpp"
#include "Zenith/Utility/Macros.hpp"

namespace zth::materials {

struct MaterialList
{
    // keep this list consistent with the material_names array

    Material plain;
    Material emerald;
    Material jade;
    Material obsidian;
    Material pearl;
    Material ruby;
    Material turquoise;
    Material brass;
    Material bronze;
    Material chrome;
    Material copper;
    Material gold;
    Material silver;
    Material black_plastic;
    Material cyan_plastic;
    Material green_plastic;
    Material red_plastic;
    Material white_plastic;
    Material yellow_plastic;
    Material black_rubber;
    Material cyan_rubber;
    Material green_rubber;
    Material red_rubber;
    Material white_rubber;
    Material yellow_rubber;

    const Material* const materials_start = &plain;
    const Material* const materials_end = &yellow_rubber + 1;

    explicit MaterialList();
    ZTH_NO_COPY_NO_MOVE(MaterialList)
    ~MaterialList() = default;

    [[nodiscard]] auto operator[](usize index) const -> const Material&;
    [[nodiscard]] auto size() const -> usize;

    [[nodiscard]] auto begin() const { return materials_start; }
    [[nodiscard]] auto end() const { return materials_end; }
};

constexpr std::array material_names = {
    "plain",        "emerald",       "jade",         "obsidian",      "pearl",          "ruby",         "turquoise",
    "brass",        "bronze",        "chrome",       "copper",        "gold",           "silver",       "black_plastic",
    "cyan_plastic", "green_plastic", "red_plastic",  "white_plastic", "yellow_plastic", "black_rubber", "cyan_rubber",
    "green_rubber", "red_rubber",    "white_rubber", "yellow_rubber",
};

auto load_materials() -> void;
auto unload_materials() -> void;

[[nodiscard]] auto materials() -> const MaterialList&;

[[nodiscard]] auto plain() -> const Material&;
[[nodiscard]] auto emerald() -> const Material&;
[[nodiscard]] auto jade() -> const Material&;
[[nodiscard]] auto obsidian() -> const Material&;
[[nodiscard]] auto pearl() -> const Material&;
[[nodiscard]] auto ruby() -> const Material&;
[[nodiscard]] auto turquoise() -> const Material&;
[[nodiscard]] auto brass() -> const Material&;
[[nodiscard]] auto bronze() -> const Material&;
[[nodiscard]] auto chrome() -> const Material&;
[[nodiscard]] auto copper() -> const Material&;
[[nodiscard]] auto gold() -> const Material&;
[[nodiscard]] auto silver() -> const Material&;
[[nodiscard]] auto black_plastic() -> const Material&;
[[nodiscard]] auto cyan_plastic() -> const Material&;
[[nodiscard]] auto green_plastic() -> const Material&;
[[nodiscard]] auto red_plastic() -> const Material&;
[[nodiscard]] auto white_plastic() -> const Material&;
[[nodiscard]] auto yellow_plastic() -> const Material&;
[[nodiscard]] auto black_rubber() -> const Material&;
[[nodiscard]] auto cyan_rubber() -> const Material&;
[[nodiscard]] auto green_rubber() -> const Material&;
[[nodiscard]] auto red_rubber() -> const Material&;
[[nodiscard]] auto white_rubber() -> const Material&;
[[nodiscard]] auto yellow_rubber() -> const Material&;

} // namespace zth::materials
