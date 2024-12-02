#pragma once

#include <spdlog/spdlog.h>

#include <string>

#include "Zenith/Core/Typedefs.hpp"

namespace zth {

struct Size
{
    u32 width;
    u32 height;
};

} // namespace zth

template<> struct fmt::formatter<zth::Size> : formatter<std::string>
{
    static auto format(const zth::Size& size, format_context& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "{{ .width = {}, .height = {} }}", size.width, size.height);
    }
};
