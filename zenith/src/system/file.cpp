#include "zenith/system/file.hpp"

namespace zth::fs {

auto write_to(const std::filesystem::path& path, std::span<const byte> data, std::ios::openmode mode) -> bool
{
    // @robustness: std::filesystem::path::string() throws.

    Defer log_error{ [&] { ZTH_INTERNAL_ERROR("[Filesystem] Couldn't write to file: \"{}\".", path.string()); } };

    std::error_code error_code;
    std::filesystem::create_directories(path, error_code);

    if (error_code)
        return false;

    std::ofstream file{ path, mode };

    if (!file.is_open())
        return false;

    file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size_bytes()));

    if (file.fail())
        return false;

    log_error.dismiss();
    return true;
}

auto extract_filename(const std::filesystem::path& path) -> Optional<String>
{
    // @robustness: string() throws.
    auto filename = path.filename().string();
    return filename.empty() ? nil : zth::make_optional(filename);
}

} // namespace zth::fs
