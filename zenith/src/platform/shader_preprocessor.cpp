#include "zenith/platform/shader_preprocessor.hpp"

#include "zenith/core/assert.hpp"
#include "zenith/embedded/shaders.hpp"
#include "zenith/fs/fs.hpp"
#include "zenith/log/logger.hpp"
#include "zenith/util/defer.hpp"

using namespace std::string_view_literals;

namespace zth {

namespace {

constexpr auto include_str = "#include"sv;

} // namespace

StringHashMap<std::string> ShaderPreprocessor::_sources;

auto ShaderPreprocessor::init() -> void
{
    // @todo: remove test_glsl
    add_source("test.glsl", embedded::shaders::test_glsl);
    add_source("zth_defines.glsl", embedded::shaders::defines_glsl);
    ZTH_CORE_INFO("Shader preprocessor initialized.");
}

auto ShaderPreprocessor::shut_down() -> void
{
    _sources.clear();
    ZTH_CORE_INFO("Shader preprocessor shut down.");
}

auto ShaderPreprocessor::preprocess(std::string_view source) -> std::expected<std::string, PreprocessShaderError>
{
    return preprocess_impl(source);
}

auto ShaderPreprocessor::add_source(std::string_view name, std::string_view source) -> bool
{
    auto [_, success] = _sources.emplace(name, source);

    if (!success)
        ZTH_CORE_DEBUG("[Shader Preprocessor] Couldn't add shader source \"{}\".", name);

    return success;
}

auto ShaderPreprocessor::add_source(std::string_view name, std::string&& source) -> bool
{
    auto [_, success] = _sources.emplace(name, std::move(source));

    if (!success)
        ZTH_CORE_DEBUG("[Shader Preprocessor] Couldn't add shader source \"{}\".", name);

    return success;
}

auto ShaderPreprocessor::add_source_from_file(const std::filesystem::path& path) -> bool
{
    auto filename = fs::extract_filename(path);

    if (!filename)
    {
        // @speed: string() throws
        ZTH_CORE_DEBUG("[Shader Preprocessor] Couldn't add shader source from file \"{}\".", path.string());
        return false;
    }

    return add_source_from_file(*filename, path);
}

auto ShaderPreprocessor::add_source_from_file(std::string_view name, const std::filesystem::path& path) -> bool
{
    auto data = fs::load_to_string(path);

    if (!data)
    {
        // @speed: string() throws
        ZTH_CORE_DEBUG("[Shader Preprocessor] Couldn't add shader source \"{}\" from file \"{}\".", name,
                       path.string());
        return false;
    }

    return add_source(name, *data);
}

auto ShaderPreprocessor::get_source(std::string_view name) -> std::optional<StringRef>
{
    if (auto kv = _sources.find(name); kv != _sources.end())
    {
        auto& [_, source] = *kv;
        return source;
    }

    ZTH_CORE_DEBUG("[Shader Preprocessor] Couldn't get shader source \"{}\".", name);
    return {};
}

auto ShaderPreprocessor::remove_source(std::string_view name) -> bool
{
    auto elems_erased = _sources.erase(name);
    return elems_erased > 0;
}

ShaderPreprocessor::ShaderPreprocessor(std::string_view source, u16 recursion_depth)
    : _rest(source), _current_recursion_depth(recursion_depth)
{
    update_rest_in_line();
}

auto ShaderPreprocessor::preprocess_impl(std::string_view source,
                                         u16 recursion_depth) -> std::expected<std::string, PreprocessShaderError>
{
    ShaderPreprocessor preprocessor(source, recursion_depth);
    return preprocessor.preprocess();
}

auto ShaderPreprocessor::preprocess() -> std::expected<std::string, PreprocessShaderError>
{
    if (_current_recursion_depth >= max_recursion_depth)
        return std::unexpected{ PreprocessShaderError{ .line_info = std::nullopt,
                                                       .description = "Circular dependency detected" } };

    while (advance_until_directive_or_comment())
    {
        // we're at "//" or '#'
        auto result = process_directive_or_comment();

        if (!result)
            return std::unexpected{ std::move(result).error() };
    }

    return std::move(_result_buffer).str();
}

auto ShaderPreprocessor::process_directive_or_comment() -> std::expected<Success, PreprocessShaderError>
{
    // we're at "//" or '#'

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

    return Success{};
}

auto ShaderPreprocessor::resolve_preprocessor_directive() -> std::expected<Success, PreprocessShaderError>
{
    // we're at '#'

    if (_rest_in_line.find(include_str) != std::string_view::npos)
    {
        return resolve_include_directive();
    }
    else
    {
        // a directive other than #include; leave it be
        advance_line();
        return Success{};
    }
}

auto ShaderPreprocessor::resolve_include_directive() -> std::expected<Success, PreprocessShaderError>
{
    // we're at '#'

    // skip until we're at the source name
    skip(include_str.size());
    skip_whitespace();

    auto source_name = extract_source_name_from_line();

    if (!source_name)
    {
        constexpr auto message = "Invalid #include directive";
        return std::unexpected{ PreprocessShaderError{ .line_info = line_info(), .description = message } };
    }

    auto included_source = get_source(*source_name);

    if (!included_source)
    {
        auto message = fmt::format("Source {} not present in shader preprocessor's source list", *source_name);
        return std::unexpected{ PreprocessShaderError{ .line_info = line_info(), .description = std::move(message) } };
    }

    auto preprocessed_included_source = preprocess_impl(included_source->get(), _current_recursion_depth + 1);

    if (!preprocessed_included_source)
        return std::unexpected{ std::move(preprocessed_included_source).error() };

    _result_buffer << *preprocessed_included_source;
    skip(source_name->size() + 2);
    return Success{};
}

auto ShaderPreprocessor::at(usize index) const -> std::optional<char>
{
    if (index >= _rest.size())
        return {};

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

// returns true if we managed to get to a directive or a comment
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

// pushes count characters into _result_buffer
auto ShaderPreprocessor::push(usize count) -> void
{
    _result_buffer << _rest.substr(0, count);
    _rest = _rest.substr(count);
}

auto ShaderPreprocessor::update_rest_in_line() -> void
{
    auto endline_pos = _rest.find('\n');

    if (endline_pos != std::string_view::npos)
        endline_pos++; // don't skip the '\n'

    _rest_in_line = _rest.substr(0, endline_pos);
}

auto ShaderPreprocessor::update_rest_in_line_after_advancing_or_skipping(usize advanced_or_skipped_count) -> void
{
    if (advanced_or_skipped_count >= _rest_in_line.size())
        update_rest_in_line();
    else
        _rest_in_line = _rest_in_line.substr(advanced_or_skipped_count);
}

auto ShaderPreprocessor::extract_source_name_from_line() const -> std::optional<std::string_view>
{
    // at this point _rest_in_line should contain only the source name i.e. "src.glsl" or <src.glsl>

    if (_rest_in_line.size() < 2)
        return {};

    std::size_t source_name_end_pos = std::string_view::npos;

    if (auto first_char = _rest_in_line.front(); first_char == '\"')
        source_name_end_pos = _rest_in_line.find('\"', 1);
    else if (first_char == '<')
        source_name_end_pos = _rest_in_line.find('>', 1);

    if (source_name_end_pos == std::string_view::npos)
        return {};

    auto source_name_length = source_name_end_pos - 1;
    return _rest_in_line.substr(1, source_name_length);
}

} // namespace zth
