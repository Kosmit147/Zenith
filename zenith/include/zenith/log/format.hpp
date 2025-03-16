#pragma once

#include <spdlog/fmt/fmt.h>

#include "zenith/stl/string.hpp"

#define ZTH_FORMAT(...) ::fmt::format(__VA_ARGS__)

#define ZTH_DECLARE_FORMATTER(type)                                                                                    \
    template<> struct ::fmt::formatter<type> : formatter<::zth::String>                                                \
    {                                                                                                                  \
        static auto format(const type&, format_context& ctx) -> decltype(ctx.out());                                   \
    }

#define ZTH_DEFINE_FORMATTER(type, var)                                                                                \
    auto ::fmt::formatter<type>::format(const type& var, format_context& ctx)->decltype(ctx.out())

#define ZTH_FORMAT_OUT(...) ::fmt::format_to(ctx.out(), __VA_ARGS__)
