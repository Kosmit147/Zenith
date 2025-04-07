#include "zenith/fs/file.hpp"

namespace zth::fs {

auto extract_filename(const std::filesystem::path& path) -> Optional<String>
{
    // @robustness: string() throws.
    auto filename = path.filename().string();
    return filename.empty() ? nil : zth::make_optional(filename);
}

template auto load_to<String>(const std::filesystem::path&) -> Optional<String>;
template auto load_to<Vector<u8>>(const std::filesystem::path&) -> Optional<Vector<u8>>;

template auto load_to<TemporaryString>(const std::filesystem::path&) -> Optional<TemporaryString>;
template auto load_to<TemporaryVector<u8>>(const std::filesystem::path&) -> Optional<TemporaryVector<u8>>;

} // namespace zth::fs
