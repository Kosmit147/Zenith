#pragma once

#include <cstdlib>
#include <iostream>
#include <source_location>

#include "zenith/util/macros.hpp"

#if !defined(ZTH_DIST_BUILD)

#define ZTH_ASSERT(expr)                                                                                               \
    {                                                                                                                  \
        if ((expr)) [[likely]]                                                                                         \
        {}                                                                                                             \
        else [[unlikely]]                                                                                              \
        {                                                                                                              \
            std::source_location location = std::source_location::current();                                           \
            std::cerr << location.file_name() << '(' << location.line() << ':' << location.column() << ") `"           \
                      << location.function_name() << "`:\nAssertion failed: (" << #expr << ")\n";                      \
            ZTH_DEBUG_BREAK;                                                                                           \
            std::abort();                                                                                              \
        }                                                                                                              \
    }                                                                                                                  \
    ZTH_NOP

#else

#define ZTH_ASSERT(expr) ZTH_NOP

#endif
