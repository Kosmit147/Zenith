#include "zenith/fs/file.hpp"

#include "zenith/log/logger.hpp"

namespace zth::fs {

auto load_to_string(const std::filesystem::path& path) -> std::optional<std::string>
{
    // @speed: Do an optimization pass.
    // @robustness: std::filesystem::exists and std::filesystem::path::string() throw.

    if (!std::filesystem::exists(path))
    {
        ZTH_CORE_ERROR("[Filesystem] Couldn't load file from path: \"{}\".", path.string());
        return {};
    }

    std::ifstream file(path);

    if (!file.is_open())
    {
        ZTH_CORE_ERROR("[Filesystem] Couldn't load file from path: \"{}\".", path.string());
        return {};
    }

    std::string result;
    result.reserve(std::filesystem::file_size(path));
    result.insert(result.end(), std::istreambuf_iterator{ file }, {});

    return std::make_optional(result);
}

auto load_raw(const std::filesystem::path& path) -> std::optional<std::vector<u8>>
{
    // @speed: Do an optimization pass.
    // @robustness: std::filesystem::exists and std::filesystem::path::string() throw.

    if (!std::filesystem::exists(path))
    {
        // @robustness: string() throws.
        ZTH_CORE_ERROR("[Filesystem] Couldn't load file from path: \"{}\".", path.string());
        return {};
    }

    std::ifstream file(path);

    if (!file.is_open())
    {
        // @robustness: string() throws.
        ZTH_CORE_ERROR("[Filesystem] Couldn't load file from path: \"{}\".", path.string());
        return {};
    }

    std::vector<u8> result;
    result.reserve(std::filesystem::file_size(path));
    result.insert(result.end(), std::istreambuf_iterator{ file }, {});

    return std::make_optional(result);
}

auto extract_filename(const std::filesystem::path& path) -> std::optional<std::string>
{
    // @robustness: string() throws.
    auto filename = path.filename().string();
    return filename.empty() ? std::nullopt : std::make_optional(filename);
}

} // namespace zth::fs
