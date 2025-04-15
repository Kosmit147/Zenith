#include "zenith/system/file.hpp"

namespace zth::fs {

auto write_to(const std::filesystem::path& path, const void* data, usize data_size_bytes, std::ios::openmode mode)
    -> bool
{
    // @robustness: std::filesystem::exists() and std::filesystem::path::string() throw.

    auto log_error = [&] { ZTH_INTERNAL_ERROR("[Filesystem] Couldn't write to file: \"{}\".", path.string()); };

    if (!std::filesystem::exists(path))
    {
        log_error();
        return false;
    }

    std::ofstream file(path, mode);

    if (!file.is_open())
    {
        log_error();
        return false;
    }

    file.write(static_cast<const char*>(data), static_cast<std::streamsize>(data_size_bytes));

    if (!file.good())
    {
        log_error();
        return false;
    }

    return true;
}

auto extract_filename(const std::filesystem::path& path) -> Optional<String>
{
    // @robustness: string() throws.
    auto filename = path.filename().string();
    return filename.empty() ? nil : zth::make_optional(filename);
}

} // namespace zth::fs
