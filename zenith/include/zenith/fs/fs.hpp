#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "zenith/core/typedefs.hpp"

namespace zth::fs {

[[nodiscard]] auto load_to_string(const std::filesystem::path& path) -> std::optional<std::string>;
[[nodiscard]] auto load_raw(const std::filesystem::path& path) -> std::optional<std::vector<u8>>;

[[nodiscard]] auto extract_filename(const std::filesystem::path& path) -> std::optional<std::string>;

} // namespace zth::fs
