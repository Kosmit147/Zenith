#pragma once

#include <spdlog/fmt/fmt.h>

#include <iterator>
#include <type_traits>
#include <utility>

#include "zenith/stl/string.hpp"

// Must be used together with ZTH_DEFINE_FORMATTER macro.
#define ZTH_DECLARE_FORMATTER(type)                                                                                    \
    template<> struct ::fmt::formatter<type> : formatter<::zth::StringView>                                            \
    {                                                                                                                  \
        static auto format(const type&, format_context& ctx) -> decltype(ctx.out());                                   \
    }

// Use ZTH_FORMAT_OUT macro to format to the output. Must be used only once in an implementation file.
#define ZTH_DEFINE_FORMATTER(type, var)                                                                                \
    auto ::fmt::formatter<type>::format(const type& var, format_context& ctx)->decltype(ctx.out())

#define ZTH_FORMAT_OUT(...) ::fmt::format_to(ctx.out(), __VA_ARGS__)

namespace zth {

template<typename... Args> [[nodiscard]] auto format(fmt::format_string<Args...> fmt, Args&&... args) -> String
{
    return fmt::format(fmt, std::forward<decltype(args)>(args)...);
}

template<std::output_iterator It, typename... Args>
auto format_to(It&& out, fmt::format_string<Args...> fmt, Args&&... args) -> std::remove_cvref_t<It>
{
    return fmt::format_to(std::forward<decltype(out)>(out), fmt, std::forward<decltype(args)>(args)...);
}

} // namespace zth
