#pragma once

#include <array>

#include "Zenith/Graphics/Material.hpp"

namespace zth::materials {

auto init_materials() -> void;

// TODO: these should be const
extern Material plain;
extern Material emerald;
extern Material jade;
extern Material obsidian;
extern Material pearl;
extern Material ruby;
extern Material turquoise;
extern Material brass;
extern Material bronze;
extern Material chrome;
extern Material copper;
extern Material gold;
extern Material silver;
extern Material black_plastic;
extern Material cyan_plastic;
extern Material green_plastic;
extern Material red_plastic;
extern Material white_plastic;
extern Material yellow_plastic;
extern Material black_rubber;
extern Material cyan_rubber;
extern Material green_rubber;
extern Material red_rubber;
extern Material white_rubber;
extern Material yellow_rubber;

constexpr std::array materials{ &plain,         &emerald,      &jade,          &obsidian,       &pearl,
                                &ruby,          &turquoise,    &brass,         &bronze,         &chrome,
                                &copper,        &gold,         &silver,        &black_plastic,  &cyan_plastic,
                                &green_plastic, &red_plastic,  &white_plastic, &yellow_plastic, &black_rubber,
                                &cyan_rubber,   &green_rubber, &red_rubber,    &white_rubber,   &yellow_rubber };

} // namespace zth::materials
