#pragma once

#include <filesystem>
#include <ranges>

#include "zenith/core/typedefs.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/memory/temporary_storage.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/stl/vector.hpp"
#include "zenith/util/optional.hpp"

namespace zth::fs {

template<std::ranges::range Container>
    requires(std::ranges::output_range<Container, std::ranges::range_value_t<Container>>)
auto load_to(const std::filesystem::path& path) -> Optional<Container>
{
    // @todo: Optimization pass.
    // @robustness: std::filesystem::exists() and std::filesystem::path::string() throw.

    if (!std::filesystem::exists(path))
    {
        ZTH_INTERNAL_ERROR("[Filesystem] Couldn't load file from path: \"{}\".", path.string());
        return nil;
    }

    std::ifstream file(path);

    if (!file.is_open())
    {
        ZTH_INTERNAL_ERROR("[Filesystem] Couldn't load file from path: \"{}\".", path.string());
        return nil;
    }

    Container result;
    result.reserve(std::filesystem::file_size(path));
    result.insert(result.end(), std::istreambuf_iterator{ file }, {});

    return zth::make_optional(result);
}

[[nodiscard]] auto extract_filename(const std::filesystem::path& path) -> Optional<String>;

extern template auto load_to<String>(const std::filesystem::path&) -> Optional<String>;
extern template auto load_to<Vector<u8>>(const std::filesystem::path&) -> Optional<Vector<u8>>;

extern template auto load_to<TemporaryString>(const std::filesystem::path&) -> Optional<TemporaryString>;
extern template auto load_to<TemporaryVector<u8>>(const std::filesystem::path&) -> Optional<TemporaryVector<u8>>;

} // namespace zth::fs
