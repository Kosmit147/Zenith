#include "zenith/renderer/shader_preprocessor.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/embedded/shaders.hpp"
#include "zenith/log/format.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/system/file.hpp"
#include "zenith/util/defer.hpp"

using namespace zth::string_view_literals;

namespace zth {

namespace {

constexpr auto include_str = "#include"_sv;

} // namespace

UnorderedMap<String, String> ShaderPreprocessor::_sources;

auto ShaderPreprocessor::init() -> Result<void, String>
{
    ZTH_INTERNAL_TRACE("Initializing shader preprocessor...");
    add_source("zth_defines.glsl", embedded::shaders::defines_glsl);
    ZTH_INTERNAL_TRACE("Shader preprocessor initialized.");
    return {};
}

auto ShaderPreprocessor::shut_down() -> void
{
    ZTH_INTERNAL_TRACE("Shutting down shader preprocessor...");
    _sources.clear();
    ZTH_INTERNAL_TRACE("Shader preprocessor shut down.");
}

auto ShaderPreprocessor::preprocess(StringView source) -> Result<String, PreprocessShaderError>
{
    return preprocess_impl(source);
}

auto ShaderPreprocessor::add_source(StringView name, StringView source) -> bool
{
    auto [_, success] = _sources.emplace(name, source);

    if (!success)
        ZTH_INTERNAL_ERROR("[Shader Preprocessor] Couldn't add shader source \"{}\".", name);

    return success;
}

auto ShaderPreprocessor::add_source(StringView name, String&& source) -> bool
{
    auto [_, success] = _sources.emplace(name, std::move(source));

    if (!success)
        ZTH_INTERNAL_ERROR("[Shader Preprocessor] Couldn't add shader source \"{}\".", name);

    return success;
}

auto ShaderPreprocessor::add_source_from_file(const std::filesystem::path& path) -> bool
{
    auto filename = fs::extract_filename(path);

    if (!filename)
    {
        // @robustness: string() throws.
        ZTH_INTERNAL_ERROR("[Shader Preprocessor] Couldn't add shader source from file \"{}\".", path.string());
        return false;
    }

    return add_source_from_file(*filename, path);
}

auto ShaderPreprocessor::add_source_from_file(StringView name, const std::filesystem::path& path) -> bool
{
    auto data = fs::read_to<String>(path);

    if (!data)
    {
        // @robustness: string() throws.
        ZTH_INTERNAL_ERROR("[Shader Preprocessor] Couldn't add shader source \"{}\" from file \"{}\".", name,
                           path.string());
        return false;
    }

    return add_source(name, std::move(*data));
}

auto ShaderPreprocessor::get_source(StringView name) -> Optional<Reference<const String>>
{
    if (auto kv = _sources.find(name); kv != _sources.end())
    {
        auto& [_, source] = *kv;
        return source;
    }

    ZTH_INTERNAL_ERROR("[Shader Preprocessor] Couldn't get shader source \"{}\".", name);
    return nil;
}

auto ShaderPreprocessor::remove_source(StringView name) -> bool
{
    auto elems_erased = _sources.erase(name);
    return elems_erased != 0;
}

ShaderPreprocessor::ShaderPreprocessor(StringView source, u16 recursion_depth)
    : _rest(source), _current_recursion_depth(recursion_depth)
{
    update_rest_in_line();
}

auto ShaderPreprocessor::preprocess_impl(StringView source, u16 recursion_depth)
    -> Result<String, PreprocessShaderError>
{
    ShaderPreprocessor preprocessor(source, recursion_depth);
    return preprocessor.preprocess();
}

auto ShaderPreprocessor::preprocess() -> Result<String, PreprocessShaderError>
{
    if (_current_recursion_depth >= max_recursion_depth)
        return Error{ PreprocessShaderError{ .line_info = nil, .description = "Circular dependency detected." } };

    while (advance_until_directive_or_comment())
    {
        // We're at "//" or '#'.
        auto result = process_directive_or_comment();

        if (!result)
            return Error{ std::move(result).error() };
    }

    return std::move(_result_buffer).str();
}

auto ShaderPreprocessor::process_directive_or_comment() -> Result<void, PreprocessShaderError>
{
    // We're at "//" or '#'.

    auto character = _rest_in_line.front();

    if (character == '/')
    {
        advance_line();
    }
    else if (character == '#')
    {
        auto result = resolve_preprocessor_directive();

        if (!result)
            return result;
    }
    else
    {
        ZTH_ASSERT(false);
    }

    return {};
}

auto ShaderPreprocessor::resolve_preprocessor_directive() -> Result<void, PreprocessShaderError>
{
    // We're at '#'.

    if (_rest_in_line.find(include_str) != StringView::npos)
    {
        return resolve_include_directive();
    }
    else
    {
        // A directive other than #include; leave it be.
        advance_line();
        return {};
    }
}

auto ShaderPreprocessor::resolve_include_directive() -> Result<void, PreprocessShaderError>
{
    // We're at '#'.

    // Skip until we're at the source name.
    skip(include_str.size());
    skip_whitespace();

    auto source_name = extract_source_name_from_line();

    if (!source_name)
    {
        constexpr auto message = "Invalid #include directive"_sv;
        return Error{ PreprocessShaderError{ .line_info = line_info(), .description = TemporaryString{ message } } };
    }

    auto included_source = get_source(*source_name);

    if (!included_source)
    {
        auto message = format_to_temporary("Source {} not present in shader preprocessor's source list", *source_name);
        return Error{ PreprocessShaderError{ .line_info = line_info(), .description = std::move(message) } };
    }

    auto preprocessed_included_source = preprocess_impl(included_source->get(), _current_recursion_depth + 1);

    if (!preprocessed_included_source)
        return Error{ std::move(preprocessed_included_source).error() };

    _result_buffer << *preprocessed_included_source;
    skip(source_name->size() + 2);
    return {};
}

auto ShaderPreprocessor::at(usize index) const -> Optional<char>
{
    if (index >= _rest.size())
        return nil;

    return _rest[index];
}

auto ShaderPreprocessor::line_info() const -> LineInfo
{
    return LineInfo{ .line = _current_line, .col = _current_col };
}

auto ShaderPreprocessor::advance_by(usize count) -> void
{
    count = std::min(_rest.size(), count);

    for (usize i = 0; i < count; i++)
    {
        if (_rest[i] == '\n')
        {
            _current_line++;
            _current_col = 0;
            continue;
        }

        _current_col++;
    }

    push(count);
    update_rest_in_line_after_advancing_or_skipping(count);
}

auto ShaderPreprocessor::advance_line() -> void
{
    advance_by(_rest_in_line.size());
}

// Returns true if we managed to get to a directive or a comment.
auto ShaderPreprocessor::advance_until_directive_or_comment() -> bool
{
    usize i = 0;

    Defer push_i_characters{ [&] {
        push(i);
        update_rest_in_line_after_advancing_or_skipping(i);
    } };

    for (; i < _rest.size(); i++)
    {
        auto character = _rest[i];

        if (character == '\n')
        {
            _current_line++;
            _current_col = 0;
            continue;
        }
        else if (character == '/')
        {
            if (auto next_character = at(i + 1); *next_character == '/')
                return true;
        }
        else if (character == '#')
        {
            return true;
        }

        _current_col++;
    }

    return false;
}

auto ShaderPreprocessor::skip(usize count) -> void
{
    count = std::min(_rest.size(), count);

    for (usize i = 0; i < count; i++)
    {
        _current_col++;

        if (_rest[i] == '\n')
        {
            _current_line++;
            _current_col = 0;
        }
    }

    _rest = _rest.substr(count);
    update_rest_in_line_after_advancing_or_skipping(count);
}

auto ShaderPreprocessor::skip_whitespace() -> void
{
    usize count = 0;

    while (count < _rest_in_line.size() && std::isspace(_rest_in_line[count]))
        count++;

    skip(count);
}

auto ShaderPreprocessor::push(usize count) -> void
{
    _result_buffer << _rest.substr(0, count);
    _rest = _rest.substr(count);
}

auto ShaderPreprocessor::update_rest_in_line() -> void
{
    auto endline_pos = _rest.find('\n');

    if (endline_pos != StringView::npos)
        endline_pos++; // Don't skip the '\n'.

    _rest_in_line = _rest.substr(0, endline_pos);
}

auto ShaderPreprocessor::update_rest_in_line_after_advancing_or_skipping(usize advanced_or_skipped_count) -> void
{
    if (advanced_or_skipped_count >= _rest_in_line.size())
        update_rest_in_line();
    else
        _rest_in_line = _rest_in_line.substr(advanced_or_skipped_count);
}

auto ShaderPreprocessor::extract_source_name_from_line() const -> Optional<StringView>
{
    // At this point _rest_in_line should contain only the source name i.e. "src.glsl" or <src.glsl>.

    if (_rest_in_line.size() < 2)
        return nil;

    auto source_name_end_pos = StringView::npos;

    if (auto first_char = _rest_in_line.front(); first_char == '\"')
        source_name_end_pos = _rest_in_line.find('\"', 1);
    else if (first_char == '<')
        source_name_end_pos = _rest_in_line.find('>', 1);

    if (source_name_end_pos == StringView::npos)
        return nil;

    auto source_name_length = source_name_end_pos - 1;
    return _rest_in_line.substr(1, source_name_length);
}

} // namespace zth

ZTH_DEFINE_FORMATTER(zth::LineInfo, line_info)
{
    return ZTH_FORMAT_OUT("({}:{})", line_info.line, line_info.col);
}

ZTH_DEFINE_FORMATTER(zth::PreprocessShaderError, error)
{
    if (error.line_info)
        return ZTH_FORMAT_OUT("{}: {}", *error.line_info, error.description);
    else
        return ZTH_FORMAT_OUT("{}", error.description);
}
