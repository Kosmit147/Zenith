#pragma once

#include <spdlog/fmt/fmt.h>

#include <string>

#define ZTH_FORMAT(...) ::fmt::format(__VA_ARGS__)

#define ZTH_DECLARE_FORMATTER(type)                                                                                    \
    template<> struct ::fmt::formatter<type> : formatter<::std::string>                                                \
    {                                                                                                                  \
        static auto format(const type&, format_context& ctx) -> decltype(ctx.out());                                   \
    }

#define ZTH_DEFINE_FORMATTER(type, var)                                                                                \
    auto ::fmt::formatter<type>::format(const type& var, format_context& ctx)->decltype(ctx.out())

#define ZTH_FORMAT_OUT(...) return ::fmt::format_to(ctx.out(), __VA_ARGS__)
