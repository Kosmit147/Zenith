#pragma once

#include <cstdlib>
#include <iostream>
#include <source_location>

#include "zenith/util/macros.hpp"

// We're using <iostream> instead of <print> to print assertion messages because it's more efficient and generates less
// code.

#define ZTH_INTERNAL_ASSERT_IMPL(...)                                                                                  \
    {                                                                                                                  \
        if ((__VA_ARGS__)) [[likely]]                                                                                  \
        {}                                                                                                             \
        else [[unlikely]]                                                                                              \
        {                                                                                                              \
            std::source_location location = std::source_location::current();                                           \
            std::cerr << location.file_name() << '(' << location.line() << ':' << location.column() << ") `"           \
                      << location.function_name() << "`:\nAssertion failed: (" << #__VA_ARGS__ << ")\n";               \
            ZTH_DEBUG_BREAK;                                                                                           \
            std::abort();                                                                                              \
        }                                                                                                              \
    }                                                                                                                  \
    ZTH_NOP

#define ZTH_RUNTIME_ASSERT(...) ZTH_INTERNAL_ASSERT_IMPL(__VA_ARGS__)

#if defined(ZTH_ASSERTIONS)

#define ZTH_ASSERT(...) ZTH_INTERNAL_ASSERT_IMPL(__VA_ARGS__)

#else

#define ZTH_ASSERT(...) ZTH_NOP

#endif
