#pragma once

#include <filesystem>
#include <fstream>
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
auto read_to(const std::filesystem::path& path, std::ios::openmode mode = std::ios::in) -> Optional<Container>
{
    // @robustness: std::filesystem::exists() and std::filesystem::path::string() throw.

    auto log_error = [&] { ZTH_INTERNAL_ERROR("[Filesystem] Couldn't read from file: \"{}\".", path.string()); };

    if (!std::filesystem::exists(path))
    {
        log_error();
        return nil;
    }

    std::ifstream file(path, mode);

    if (!file.is_open())
    {
        log_error();
        return nil;
    }

    Container result;
    result.reserve(std::filesystem::file_size(path));
    result.insert(result.end(), std::istreambuf_iterator{ file }, {});

    if (!file.good())
    {
        log_error();
        return nil;
    }

    return zth::make_optional(result);
}

auto write_to(const std::filesystem::path& path, const void* data, usize data_size_bytes,
              std::ios::openmode mode = std::ios::out) -> bool;

auto write_to(const std::filesystem::path& path, std::ranges::contiguous_range auto&& data,
              std::ios::openmode mode = std::ios::out) -> bool
{
    return write_to(path, std::data(data), std::size(data) * sizeof(std::ranges::range_value_t<decltype(data)>), mode);
}

[[nodiscard]] auto extract_filename(const std::filesystem::path& path) -> Optional<String>;

} // namespace zth::fs
