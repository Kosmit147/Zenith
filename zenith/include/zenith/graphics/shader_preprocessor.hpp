#pragma once

#include <filesystem>
#include <sstream>
#include <string>
#include <string_view>

#include "zenith/core/typedefs.hpp"
#include "zenith/log/format.hpp"
#include "zenith/stl/string_map.hpp"
#include "zenith/util/optional.hpp"
#include "zenith/util/reference.hpp"
#include "zenith/util/result.hpp"

namespace zth {

struct LineInfo
{
    u32 line;
    u32 col;
};

struct PreprocessShaderError
{
    Optional<LineInfo> line_info;
    std::string description;
};

class ShaderPreprocessor
{
public:
    static constexpr u16 max_recursion_depth = 30;

public:
    ShaderPreprocessor() = delete;

    static auto init() -> void;
    static auto shut_down() -> void;

    [[nodiscard]] static auto preprocess(std::string_view source) -> Result<std::string, PreprocessShaderError>;

    static auto add_source(std::string_view name, std::string_view source) -> bool;
    static auto add_source(std::string_view name, std::string&& source) -> bool;
    static auto add_source_from_file(const std::filesystem::path& path) -> bool;
    static auto add_source_from_file(std::string_view name, const std::filesystem::path& path) -> bool;

    [[nodiscard]] static auto get_source(std::string_view name) -> Optional<Reference<const std::string>>;

    static auto remove_source(std::string_view name) -> bool;

private:
    std::string_view _rest;
    std::string_view _rest_in_line;

    u32 _current_col = 0;
    u32 _current_line = 0;

    u16 _current_recursion_depth = 0;

    std::ostringstream _result_buffer;

    static StringHashMap<std::string> _sources;

private:
    explicit ShaderPreprocessor(std::string_view source, u16 recursion_depth = 0);

    [[nodiscard]] static auto preprocess_impl(std::string_view source, u16 recursion_depth = 0)
        -> Result<std::string, PreprocessShaderError>;
    [[nodiscard]] auto preprocess() -> Result<std::string, PreprocessShaderError>;

    [[nodiscard]] auto process_directive_or_comment() -> Result<Success, PreprocessShaderError>;
    [[nodiscard]] auto resolve_preprocessor_directive() -> Result<Success, PreprocessShaderError>;
    [[nodiscard]] auto resolve_include_directive() -> Result<Success, PreprocessShaderError>;

    [[nodiscard]] auto at(usize index) const -> Optional<char>;
    [[nodiscard]] auto line_info() const -> LineInfo;

    auto advance_by(usize count) -> void;
    auto advance_line() -> void;
    auto advance_until_directive_or_comment() -> bool;

    auto skip(usize count) -> void;
    auto skip_whitespace() -> void;

    auto push(usize count) -> void;

    auto update_rest_in_line() -> void;
    auto update_rest_in_line_after_advancing_or_skipping(usize advanced_or_skipped_count) -> void;

    [[nodiscard]] auto extract_source_name_from_line() const -> Optional<std::string_view>;
};

} // namespace zth

ZTH_DECLARE_FORMATTER(zth::LineInfo);
ZTH_DECLARE_FORMATTER(zth::PreprocessShaderError);
