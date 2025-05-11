#pragma once

#include <filesystem>
#include <fstream>
#include <ranges>
#include <span>
#include <system_error>

#include "zenith/core/typedefs.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/stl/span.hpp"
#include "zenith/stl/string.hpp"
#include "zenith/stl/vector.hpp"
#include "zenith/system/temporary_storage.hpp"
#include "zenith/util/defer.hpp"
#include "zenith/util/optional.hpp"

namespace zth::fs {

// Reads from files in binary mode by default.
template<std::ranges::contiguous_range Container>
auto read_to(const std::filesystem::path& path, std::ios::openmode mode = std::ios::in | std::ios::binary)
    -> Optional<Container>
{
    // @robustness: std::filesystem::path::string() throws.

    Defer log_error{ [&] { ZTH_INTERNAL_ERROR("[Filesystem] Couldn't read from file: \"{}\".", path.string()); } };

    std::error_code error_code;
    std::filesystem::directory_entry file_entry{ path, error_code };

    if (error_code)
        return nil;

    std::ifstream file{ path, mode };

    if (!file.is_open())
        return nil;

    auto file_size = file_entry.file_size();
    Container result;

    if constexpr (sizeof(std::ranges::range_value_t<Container>) == 1)
    {
        result.resize(file_size);
        file.read(reinterpret_cast<char*>(result.data()), static_cast<std::streamsize>(file_size));

        if (file.gcount() != static_cast<std::streamsize>(file_size))
            return nil;
    }
    else
    {
        result.reserve(file_size);
        result.insert(result.end(), std::istreambuf_iterator{ file }, {});
    }

    if (file.fail())
        return nil;

    log_error.dismiss();
    return zth::make_optional(result);
}

// Writes to files in binary mode by default. Automatically creates new directories.
auto write_to(const std::filesystem::path& path, std::span<const byte> data,
              std::ios::openmode mode = std::ios::out | std::ios::binary) -> bool;

// Writes to files in binary mode by default. Automatically creates new directories.
auto write_to(const std::filesystem::path& path, std::ranges::contiguous_range auto&& data,
              std::ios::openmode mode = std::ios::out | std::ios::binary) -> bool
{
    return write_to(path, make_dynamic_span(std::as_bytes(std::span{ data })), mode);
}

[[nodiscard]] auto extract_filename(const std::filesystem::path& path) -> Optional<String>;

} // namespace zth::fs
