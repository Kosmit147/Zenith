#pragma once

#include <cstdlib>
#include <iostream>
#include <source_location>

#include "Zenith/Utility/Macros.hpp"

#ifndef ZTH_DIST_BUILD

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
    ((void)0)

#else

#define ZTH_ASSERT(expr) ((void)0)

#endif
