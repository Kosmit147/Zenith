#pragma once

#include <filesystem>
#include <string>

#include "zenith/core/typedefs.hpp"
#include "zenith/stl/vector.hpp"
#include "zenith/util/optional.hpp"

namespace zth::fs {

[[nodiscard]] auto load_to_string(const std::filesystem::path& path) -> Optional<std::string>;
[[nodiscard]] auto load_raw(const std::filesystem::path& path) -> Optional<Vector<u8>>;

[[nodiscard]] auto extract_filename(const std::filesystem::path& path) -> Optional<std::string>;

} // namespace zth::fs
